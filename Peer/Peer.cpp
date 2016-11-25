//
// Created by shehab on 11/18/16.
//

#include <ios>
#include <fstream>
#include <iostream>
#include <SQLiteCpp/Database.h>
#include "Peer.h"

// Peer Constructor initializes the CM-CLient and sets its port to default 0 while the Servers sets
// its port to defined listen port.
Peer::Peer(char *_listen_hostname, uint16_t _listen_port) :
        CM_Client(_listen_hostname, 0),
        CM_Server(_listen_hostname, _listen_port) {
    //Each object should have two threads.
    // Server thread that keeps listening infinitely.
    // Client thread which provides an interface in Qt.
    std::thread Peer_Server, Peer_Client;

}

Peer::~Peer() {

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Peer::runServer() {
    sockaddr_in sender_addr;
    while (true) {
        Message recv_message = Message();
        ssize_t bytes_read = CM_Server.recv_with_block(recv_message, sender_addr);
        handleRequest(recv_message, sender_addr);
    }

}

#pragma clang diagnostic pop

void Peer::handleRequest(Message request, sockaddr_in sender_addr) {
    // Only One case added for now.
    switch (request.getOperation()) {
        //Operation 0: Download Image
        case 0: {
            std::vector<std::string> params;  // params[0] = image_name, params[size-1] = token
            params = request.getParams();
            std::string image_name = params[0];
            long int token = stoi(params[params.size() - 1]);
            std::vector<std::string> reply_params;
            Message image_msg;
            auto n = download_image_svc(image_name, token, reply_params);
            image_msg = Message(MessageType::Reply, 0, request.getRPCId(), std::to_string(n), reply_params.size(),
                                reply_params);
            CM_Server.send_no_ack(image_msg, sender_addr);
            break;
        }

        default:
            break;
    }
}

///////////////////////////////////////////////////
//                Peer RPC Stubs                //
//////////////////////////////////////////////////
int Peer::download_image(std::string image_name, long int token, std::vector<std::string> &reply_params) {
    return 0;
}

//////////////////////////////////////////////////
//             Peer RPC Implementation          //
/////////////////////////////////////////////////
int Peer::download_image_svc(std::string image_name, long int token, std::vector<std::string> &reply_params) {

    int n = check_token(token);
    //if n = 0, success.
    //Else return -1

    if(n == 0) {
        try {
            //Opening image file to stream then storing it into a string.
            std::ifstream fin(image_name, std::ios::binary);
            std::string image_data;
            image_data.reserve(1000000);
//        Not working
//        std::copy(std::istreambuf_iterator<char>(fin),
//                  std::istreambuf_iterator<char>(),
//                  std::back_insert_iterator(image_data));

            reply_params.push_back(image_data);
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}

//////////////////////////////////////////////////
//             Registry RPC Stubs               //
/////////////////////////////////////////////////
int Peer::retrieve_token(std::string username, std::string password, long int &token) {
    return 0;
}

int Peer::view_imagelist(std::vector<std::string> &image_container, long int token) {
    return 0;
}

int Peer::add_entry(std::string image_name, long int token) {
    return 0;
}

int Peer::remove_entry(std::string image_name, long int token) {
    return 0;
}

int Peer::get_client_addr(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token) {
    return 0;
}

int Peer::check_viewImage(std::string image_id, bool &can_view, long int token) {
    return 0;
}

int Peer::check_token(long int token) {
    return 0;
}

int Peer::numbViewsLeft(std::string image_id, long int token) {
    return 0;
}

int Peer::setNumViews_EachUser(std::string image_id, int peer_token, int noViews) {
    return 0;
}

int main() {
    using namespace std;
    CM client(NULL, 0);
    std::ifstream t("/home/shadyf/Pictures/openflow.png", ios::binary);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());
    std::ofstream ofile("./doubledelight.png", std::ios::binary);
    ofile << str;
//    cout << str.size();
//    std::vector<std::string> v{str};
//    Message request(MessageType::Request, 0, 0, "null", v.size(), v);
//    std::cout << "Request Size: " << request.marshal().size() << std::endl;
//    Message reply;
//    cout << request.marshal() << endl;
//    int bytes_read = client.send_with_ack(request, reply, 500, 5, (char *) "192.168.43.232", 1234);
//    if(bytes_read >= 0) {
//        std::cout << "Reply size: " << reply.marshal().size() << std::endl;
//        std::cout << reply.marshal() << std::endl;
    }