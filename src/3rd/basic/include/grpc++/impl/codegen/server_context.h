
#ifndef GRPCXX_IMPL_CODEGEN_SERVER_CONTEXT_H
#define GRPCXX_IMPL_CODEGEN_SERVER_CONTEXT_H

#include <map>
#include <memory>
#include <vector>

#include <grpc/impl/codegen/compression_types.h>

#include <grpc++/impl/codegen/config.h>
#include <grpc++/impl/codegen/create_auth_context.h>
#include <grpc++/impl/codegen/metadata_map.h>
#include <grpc++/impl/codegen/security/auth_context.h>
#include <grpc++/impl/codegen/string_ref.h>
#include <grpc++/impl/codegen/time.h>

struct grpc_metadata;
struct grpc_call;
struct census_context;

namespace grpc {
class ClientContext;
template <class W, class R>
class ServerAsyncReader;
template <class W>
class ServerAsyncWriter;
template <class W>
class ServerAsyncResponseWriter;
template <class W, class R>
class ServerAsyncReaderWriter;
template <class R>
class ServerReader;
template <class W>
class ServerWriter;
namespace internal {
template <class W, class R>
class ServerReaderWriterBody;
}
template <class ServiceType, class RequestType, class ResponseType>
class RpcMethodHandler;
template <class ServiceType, class RequestType, class ResponseType>
class ClientStreamingHandler;
template <class ServiceType, class RequestType, class ResponseType>
class ServerStreamingHandler;
template <class ServiceType, class RequestType, class ResponseType>
class BidiStreamingHandler;
class UnknownMethodHandler;

class Call;
class CallOpBuffer;
class CompletionQueue;
class Server;
class ServerInterface;

namespace testing {
class InteropServerContextInspector;
class ServerContextTestSpouse;
}  // namespace testing

// Interface of server side rpc context.
class ServerContext {
 public:
  ServerContext();  // for async calls
  ~ServerContext();

  std::chrono::system_clock::time_point deadline() const {
    return Timespec2Timepoint(deadline_);
  }

  gpr_timespec raw_deadline() const { return deadline_; }

  void AddInitialMetadata(const grpc::string& key, const grpc::string& value);
  void AddTrailingMetadata(const grpc::string& key, const grpc::string& value);

  // IsCancelled is always safe to call when using sync API
  // When using async API, it is only safe to call IsCancelled after
  // the AsyncNotifyWhenDone tag has been delivered
  bool IsCancelled() const;

  void TryCancel() const;

  const std::multimap<grpc::string_ref, grpc::string_ref>& client_metadata()
      const {
    return *client_metadata_.map();
  }

  grpc_compression_level compression_level() const {
    return compression_level_;
  }

  void set_compression_level(grpc_compression_level level) {
    compression_level_set_ = true;
    compression_level_ = level;
  }

  bool compression_level_set() const { return compression_level_set_; }

  grpc_compression_algorithm compression_algorithm() const {
    return compression_algorithm_;
  }
  void set_compression_algorithm(grpc_compression_algorithm algorithm);

  // Set the load reporting costs in \a cost_data for the call.
  void SetLoadReportingCosts(const std::vector<grpc::string>& cost_data);

  std::shared_ptr<const AuthContext> auth_context() const {
    if (auth_context_.get() == nullptr) {
      auth_context_ = CreateAuthContext(call_);
    }
    return auth_context_;
  }

  // Return the peer uri in a string.
  // WARNING: this value is never authenticated or subject to any security
  // related code. It must not be used for any authentication related
  // functionality. Instead, use auth_context.
  grpc::string peer() const;

  const struct census_context* census_context() const;

  // Async only. Has to be called before the rpc starts.
  // Returns the tag in completion queue when the rpc finishes.
  // IsCancelled() can then be called to check whether the rpc was cancelled.
  void AsyncNotifyWhenDone(void* tag) {
    has_notify_when_done_tag_ = true;
    async_notify_when_done_tag_ = tag;
  }

  // Should be used for framework-level extensions only.
  // Applications never need to call this method.
  grpc_call* c_call() { return call_; }

 private:
  friend class ::grpc::testing::InteropServerContextInspector;
  friend class ::grpc::testing::ServerContextTestSpouse;
  friend class ::grpc::ServerInterface;
  friend class ::grpc::Server;
  template <class W, class R>
  friend class ::grpc::ServerAsyncReader;
  template <class W>
  friend class ::grpc::ServerAsyncWriter;
  template <class W>
  friend class ::grpc::ServerAsyncResponseWriter;
  template <class W, class R>
  friend class ::grpc::ServerAsyncReaderWriter;
  template <class R>
  friend class ::grpc::ServerReader;
  template <class W>
  friend class ::grpc::ServerWriter;
  template <class W, class R>
  friend class ::grpc::internal::ServerReaderWriterBody;
  template <class ServiceType, class RequestType, class ResponseType>
  friend class RpcMethodHandler;
  template <class ServiceType, class RequestType, class ResponseType>
  friend class ClientStreamingHandler;
  template <class ServiceType, class RequestType, class ResponseType>
  friend class ServerStreamingHandler;
  template <class Streamer, bool WriteNeeded>
  friend class TemplatedBidiStreamingHandler;
  friend class UnknownMethodHandler;
  friend class ::grpc::ClientContext;

  // Prevent copying.
  ServerContext(const ServerContext&);
  ServerContext& operator=(const ServerContext&);

  class CompletionOp;

  void BeginCompletionOp(Call* call);

  ServerContext(gpr_timespec deadline, grpc_metadata_array* arr);

  void set_call(grpc_call* call) { call_ = call; }

  uint32_t initial_metadata_flags() const { return 0; }

  CompletionOp* completion_op_;
  bool has_notify_when_done_tag_;
  void* async_notify_when_done_tag_;

  gpr_timespec deadline_;
  grpc_call* call_;
  CompletionQueue* cq_;
  bool sent_initial_metadata_;
  mutable std::shared_ptr<const AuthContext> auth_context_;
  MetadataMap client_metadata_;
  std::multimap<grpc::string, grpc::string> initial_metadata_;
  std::multimap<grpc::string, grpc::string> trailing_metadata_;

  bool compression_level_set_;
  grpc_compression_level compression_level_;
  grpc_compression_algorithm compression_algorithm_;
};

}  // namespace grpc

#endif  // GRPCXX_IMPL_CODEGEN_SERVER_CONTEXT_H
