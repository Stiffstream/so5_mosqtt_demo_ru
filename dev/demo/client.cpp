#include "messages.hpp"
#include "common.hpp"

#include <random>

namespace mosqtt = mosquitto_transport;

// Демонстрационный агент, который будет отсылать приветственное сообщение.
class client_t final : public so_5::agent_t {
public:
   client_t(
         context_t ctx,
         int id,
         mosqtt::instance_t mqtt,
         std::shared_ptr<spdlog::logger> logger)
      :  so_5::agent_t{std::move(ctx)}
      ,  id_{id}
      ,  mqtt_{std::move(mqtt)}
      ,  logger_{std::move(logger)} {
   }

   virtual void so_define_agent() override {
      // Подписываемся на уведомления о наличии подключения к брокеру.
      so_subscribe(mqtt_.mbox()).event(&client_t::on_broker_connected);

      demo::subscriber_t::subscribe(mqtt_, make_topic_name("/replies"),
         [&](const so_5::mbox_t & mbox) {
            so_subscribe(mbox).event(&client_t::on_reply);
         });
   }

private:
   const int id_;
   mosqtt::instance_t mqtt_;
   std::shared_ptr<spdlog::logger> logger_;

   std::string make_topic_name(const char * suffix) const {
      return std::string("/client/") + std::to_string(id_) + suffix;
   }

   void on_broker_connected(mhood_t<mosqtt::broker_connected_t>) {
      // После того, как подключились к брокеру можем сообщить о
      // своем существовании.
      demo::publisher_t::publish(
            mqtt_,
            make_topic_name("/hello"),
            demo::client_hello_t{
                  id_,
                  "Hello, World",
                  make_topic_name("/replies")});
   }

   void on_reply(mhood_t<demo::subscriber_t::msg_type> cmd) {
      logger_->trace("message received from topic: {}, payload={}",
            cmd->topic_name(),
            cmd->payload());

      const auto msg = cmd->decode<demo::server_hello_t>();

      logger_->info("hello received. greeting={}", msg.greeting_);

      // Теперь мы свою работу можем завершить. 
      logger_->warn("finishing");

      so_environment().stop();
   }
};

void launch_demo() {
   so_5::launch([&](so_5::environment_t & env) {
      auto loggers = demo::make_loggers();

      // Идентификатор для клиента будет генерироваться случайным образом.
      const int client_id = std::random_device{}() % 100000;
      loggers.app_logger_->info("client_id={}", client_id);

      auto mqtt = demo::run_transport_manager(
            env,
            "demo-client-" + std::to_string(client_id),
            loggers.mqtt_logger_);

      env.introduce_coop([&](so_5::coop_t & coop) {
         coop.make_agent<client_t>(client_id, mqtt, loggers.app_logger_);
      });
   });
}

int main() {
   try {
      launch_demo();
      return 0;
   }
   catch(const std::exception & x) {
      std::cerr << "Exception caught: " << x.what() << std::endl;
   }

   return 2;
}

