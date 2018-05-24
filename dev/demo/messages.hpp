/*
 * Сообщения, которыми будут обмениваться клиент и сервер.
 */
#pragma once

#include <json_dto/pub.hpp>

#include <mosquitto_transport/pub.hpp>

namespace demo {

// Тип-тег, который будет указывать в каком формате должны быть
// сериализованы сообщения.
struct json_format_t {};

// Удобные имена для использования.
using publisher_t = mosquitto_transport::topic_publisher_t<json_format_t>;
using subscriber_t = mosquitto_transport::topic_subscriber_t<json_format_t>;

// Сообщение о появлении нового клиента.
struct client_hello_t {
   int id_;
   std::string greeting_;
   std::string reply_topic_;

   template<typename Json_Io>
   void json_io(Json_Io & io) {
      using namespace json_dto;
      
      io & mandatory("id", id_)
         & mandatory("greeting", greeting_)
         & mandatory("reply_topic", reply_topic_);
   }
};

// Ответное сообщение от сервера.
struct server_hello_t {
   std::string greeting_;

   template<typename Json_Io>
   void json_io(Json_Io & io) {
      io & json_dto::mandatory("greeting", greeting_);
   }
};

} /* namespace demo */

// Средства для сериализации-десериализации сообщений.
namespace mosquitto_transport {

template<typename Msg>
struct encoder_t<demo::json_format_t, Msg> {
   static std::string encode(const Msg & msg) {
      return json_dto::to_json(msg);
   }
};

template<typename Msg>
struct decoder_t<demo::json_format_t, Msg> {
   static Msg decode(const std::string & json) {
      return json_dto::from_json<Msg>(json);
   }
};

} /* namespace mosquitto_transport */

