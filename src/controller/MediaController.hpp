#ifndef MediaController_hpp
#define MediaController_hpp

#define VLC

#include "../hls/Playlist.hpp"
#include "../Utils.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/web/protocol/http/outgoing/ChunkedBufferBody.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

/**
 *  EXAMPLE ApiController
 *  Basic examples of how to create ENDPOINTs
 *  More details on oatpp.io
 */

struct UserInfo{
    oatpp::String login;
};

enum class Mode{FRAMES, RECORDS};

static Mode m = Mode::RECORDS;

static std::unordered_map<std::string, UserInfo> m_userInfoByToken;

static std::unordered_map<oatpp::String, oatpp::String> users;

class MediaController : public oatpp::web::server::api::ApiController {
public:
  typedef MediaController __ControllerType;
private:
  OATPP_COMPONENT(std::shared_ptr<StaticFilesManager>, staticFileManager);
  OATPP_COMPONENT(std::shared_ptr<Playlist>, livePlaylist);
private:
  std::shared_ptr<OutgoingResponse> getStaticFileResponse(const oatpp::String& filename, const oatpp::String& rangeHeader) const;
  std::shared_ptr<OutgoingResponse> getFullFileResponse(const oatpp::String& file) const;
  std::shared_ptr<OutgoingResponse> getRangeResponse(const oatpp::String& rangeStr, const oatpp::String& file) const;
public:
  MediaController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {
     users["admin"] = "123";
  }
public:
	
  /**
   *  Inject @objectMapper component here as default parameter
   */
  static std::shared_ptr<MediaController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)){
    return std::shared_ptr<MediaController>(std::make_shared<MediaController>(objectMapper));
  }

/**
 *  Begin ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_BEGIN(ApiController)

#if defined VLC

  ENDPOINT_ASYNC("GET", "/frames", Frames) {

    ENDPOINT_ASYNC_INIT(Frames)

    static v_int64 getTime0(){
      static v_int64 time0 = getMillisTickCount();
      return time0;
    }

    Action act() override {

       auto it = m_userInfoByToken.find("admin");
       if(it != m_userInfoByToken.end()){
          m = Mode::FRAMES;
          v_int64 time = getMillisTickCount() - getTime0();
          auto buff = controller->livePlaylist->generateForTime(time, 5);
          auto response = controller->createResponse(Status::CODE_200, buff);
          response->putHeader("Accept-Ranges", "bytes");
          response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
          response->putHeader(Header::CONTENT_TYPE, "application/x-mpegURL");
          return _return(response);
       } else {
         return _return(controller->createResponse(Status::CODE_401, "Go Away!"));
       }

    }

  };

  ENDPOINT_ASYNC("GET", "/record", Record) {

    ENDPOINT_ASYNC_INIT(Record)

    static v_int64 getTime0(){
      static v_int64 time0 = getMillisTickCount();
      return time0;
    }

    Action act() override {

       auto it = m_userInfoByToken.find("admin");
       if(it != m_userInfoByToken.end()){
          m = Mode::RECORDS;
          v_int64 time = getMillisTickCount() - getTime0();
          auto buff = controller->livePlaylist->generateForTime(time, 5);
          auto response = controller->createResponse(Status::CODE_200, buff);
          response->putHeader("Accept-Ranges", "bytes");
          response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
          response->putHeader(Header::CONTENT_TYPE, "application/x-mpegURL");
          return _return(response);
       } else {
         return _return(controller->createResponse(Status::CODE_401, "Go Away!"));
       }

    }

  };

  ENDPOINT_ASYNC("GET", "/authenticate/{login}/{password}", Authenticate) {

    ENDPOINT_ASYNC_INIT(Authenticate)

    Action act() override {

      auto login = request->getPathVariable("login");
      auto password = request->getPathVariable("password");

      if(login == "admin" && password == "123") {
        oatpp::String token = "admin";
        UserInfo userInfo{login};
        m_userInfoByToken["admin"] = userInfo;
        auto response = controller->createResponse(Status::CODE_200, token);
        return _return(response);
      }

      return _return(controller->createResponse(Status::CODE_401, "Go Away!"));

    }

  };

#else

  ENDPOINT_ASYNC("GET", "/frames", Frames) {

    ENDPOINT_ASYNC_INIT(Frames)

    static v_int64 getTime0(){
      static v_int64 time0 = getMillisTickCount();
      return time0;
    }

    Action act() override {

       auto cookies = request->getHeader("Cookie");
       std::string token = parseTokenFromCookie(cookies);

       auto it = m_userInfoByToken.find(token);
       if(it != m_userInfoByToken.end()){
          m = Mode::FRAMES;
          v_int64 time = getMillisTickCount() - getTime0();
          auto buff = controller->livePlaylist->generateForTime(time, 5);
          auto response = controller->createResponse(Status::CODE_200, buff);
          response->putHeader("Accept-Ranges", "bytes");
          response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
          response->putHeader(Header::CONTENT_TYPE, "application/x-mpegURL");
          return _return(response);
       } else {
         return _return(controller->createResponse(Status::CODE_401, "Go Away!"));
       }

    }

  };

  ENDPOINT_ASYNC("GET", "/record", Record) {

    ENDPOINT_ASYNC_INIT(Record)

    static v_int64 getTime0(){
      static v_int64 time0 = getMillisTickCount();
      return time0;
    }

    Action act() override {

       auto cookies = request->getHeader("Cookie");
       std::string token = parseTokenFromCookie(cookies);

       auto it = m_userInfoByToken.find(token);
       if(it != m_userInfoByToken.end()){
          m = Mode::RECORDS;
          v_int64 time = getMillisTickCount() - getTime0();
          auto buff = controller->livePlaylist->generateForTime(time, 5);
          auto response = controller->createResponse(Status::CODE_200, buff);
          response->putHeader("Accept-Ranges", "bytes");
          response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
          response->putHeader(Header::CONTENT_TYPE, "application/x-mpegURL");
          return _return(response);
       } else {
         return _return(controller->createResponse(Status::CODE_401, "Go Away!"));
       }

    }

  };

  ENDPOINT_ASYNC("GET", "/authenticate/{login}/{password}", Authenticate) {

    ENDPOINT_ASYNC_INIT(Authenticate)

    oatpp::String generateMegaSecureRandomToken(){
      return "admin@admin";
    }

    Action act() override {

      auto login = request->getPathVariable("login");
      auto password = request->getPathVariable("password");

      auto it = users.find(login);

      if(it != users.end() && it->second == password) {
        oatpp::String token = login + generateMegaSecureRandomToken();
        UserInfo userInfo{login};
        m_userInfoByToken[token->std_str()] = userInfo;
        auto response = controller->createResponse(Status::CODE_200, token);
        response->putHeader("Set-Cookie", "token=" + token + "; Path=/");
        return _return(response);
      }

      return _return(controller->createResponse(Status::CODE_401, "Go Away!"));

    }

  };

#endif

ENDPOINT_ASYNC("GET", "/*", Static) {

    ENDPOINT_ASYNC_INIT(Static)

    Action act() override {
      auto filename = request->getPathTail();
      OATPP_ASSERT_HTTP(filename, Status::CODE_400, "Filename is empty");
      std::system("pwd > text.txt");
      std::stringstream ss;
      ss << std::ifstream("text.txt").rdbuf();
      std::string path;
      ss >> path;
      path = path.substr(0, path.find_last_of('/'));
      path += "/video/";
      std::string f_name = filename->std_str();
      std::string name = path + f_name;
      f_name = f_name.substr(0, f_name.size() - 3);
      std::string command;
      if (m == Mode::RECORDS)
      {
          std::string output_name = path + "stream/" + f_name + ".mpg";
          command = "ffmpeg -i " + name + " -vcodec copy -acodec copy " + output_name;
      }
      else
      {
          command = "ffmpeg -i " + name + " -vf fps=1 " + path + "frames/" + f_name + "_thumb%03d.jpg";
      }
      std::system(command.c_str());

      auto range = request->getHeader(Header::RANGE);

      return _return(controller->getStaticFileResponse(filename, range));

    }

  };

/**
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* MediaController_hpp */
