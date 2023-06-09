#include <iostream>
#include <memory>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "./test1.grpc.pb.h"

using std::endl;
using std::cout;
using std::string;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using test1::TestService;
using test1::Data;
using test1::MsgReply;

class Test1Client{
public:
    // create stub
    Test1Client(std::shared_ptr<Channel> channel):stub_(TestService::NewStub(channel)){}
    void GetReplyMsg()
    {
        Data data;
        MsgReply msg_reply;
        data.set_data(123);
        GetOneData(data,&msg_reply);
    }
private:
    bool GetOneData(const Data& data,MsgReply* msg_reply)
    {
        ClientContext context;
        Status status=stub_->getData(&context,data,msg_reply);
        if(!status.ok())
        {
            cout<<"GetData rpc failed."<<endl;
            return false;
        }
        if(msg_reply->message().empty())
        {
            cout<<"message empty."<<endl;
            return false;
        }
        else
        {
            cout<<"MsgReply:"<<msg_reply->message()<<endl;
        }
        return true;
    }

    std::unique_ptr<TestService::Stub> stub_;
};

int main(int argc,char** argv)
{
    // create a gRPC channel for our stub
    //grpc::CreateChannel("locakhost:50051",grpc::InsecureChannelCredentials());
    Test1Client client1(grpc::CreateChannel("localhost:50051",grpc::InsecureChannelCredentials()));
    cout<<"====================="<<endl;
    client1.GetReplyMsg();

    return 0;
}