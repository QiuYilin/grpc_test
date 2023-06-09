#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "./test1.grpc.pb.h"
#include <iostream>
#include <string>
#include <memory>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::ServerReaderWriter;
using grpc::Status;
using std::cout;
using std::endl;
using std::string;

using test1::Data;
using test1::MsgReply;
using test1::TestService;

class Test1Impl final:public TestService::Service{
public:
    Status getData(ServerContext* context,const Data* data,MsgReply* msg)override
    {
        cout<<"[get data]: "<<data->data()<<endl;
        string tmp("data received 12345");
        msg->set_message(tmp);
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_addr("0.0.0.0:50051");
    // create an instance of our service implementation class Test1Impl
    Test1Impl service;

    // Create an instance of factory ServerBuilder class
    ServerBuilder builder;

    // Specify the address and port we want to use to listen for client requests using the builder’s AddListeningPort() method.
    builder.AddListeningPort(server_addr,grpc::InsecureServerCredentials());

    // Register our service implementation with the builder.
    builder.RegisterService(&service);

    // Call BuildAndStart() on the builder to create and start an RPC server for our service.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    cout<<"Server listening on "<<server_addr<<endl;

    // Call Wait() on the server to do a blocking wait until process is killed or Shutdown() is called
    server->Wait();
}

int main(int argc,char** argv)
{
    RunServer();
    return 0;
}