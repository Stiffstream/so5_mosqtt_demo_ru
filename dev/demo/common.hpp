/*
 * Общий код, который потребуется и клиенту, и серверу.
 */
#pragma once

#include <mosquitto_transport/a_transport_manager.hpp>

namespace demo {

// Вспомогательная функция, которая будет создавать логгеров для примера.
auto make_loggers() {
   struct result_t {
      std::shared_ptr<spdlog::logger> mqtt_logger_;
      std::shared_ptr<spdlog::logger> app_logger_;
   };

   result_t loggers{
         spdlog::stdout_color_mt("mosqtt"),
         spdlog::stdout_color_mt("app")
      };

   loggers.mqtt_logger_->set_level(spdlog::level::trace);
   loggers.app_logger_->set_level(spdlog::level::trace);

   return loggers;
}

// Вспомогательная функция, которая будет запускать transport_manager.
auto run_transport_manager(
      so_5::environment_t & env,
      const std::string & client_id,
      std::shared_ptr<spdlog::logger> logger) {
   mosquitto_transport::instance_t mqtt;
   env.introduce_coop([&](so_5::coop_t & coop) {
      auto lib = coop.take_under_control(
            std::make_unique<mosquitto_transport::lib_initializer_t>());

      auto tm = coop.make_agent<mosquitto_transport::a_transport_manager_t>(
            std::ref(*lib),
            mosquitto_transport::connection_params_t{
                  client_id, "localhost", 1883u, 60u},
            std::move(logger));

      mqtt = tm->instance();
   });

   return mqtt;
}

} /* namespace demo */

