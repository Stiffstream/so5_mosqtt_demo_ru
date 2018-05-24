#include "messages.hpp"
#include "common.hpp"

namespace mosqtt = mosquitto_transport;

// Демонстрационный агент, который будет получать приветственные
// сообщения от клиентов.
class listener_t final : public so_5::agent_t {
public:
   listener_t(
         context_t ctx,
         mosqtt::instance_t mqtt,
         std::shared_ptr<spdlog::logger> logger)
      :  so_5::agent_t{std::move(ctx)}
      ,  mqtt_{std::move(mqtt)}
      ,  logger_{std::move(logger)}
      {}

   virtual void so_define_agent() override {
      demo::subscriber_t::subscribe(mqtt_, "/client/+/hello",
         [&](const so_5::mbox_t & mbox) {
            so_subscribe(mbox).event(&listener_t::on_hello);
         });
   }

private:
   mosqtt::instance_t mqtt_;
   std::shared_ptr<spdlog::logger> logger_;

   void on_hello(mhood_t<demo::subscriber_t::msg_type> cmd) {
      logger_->trace("message received from topic: {}, payload={}",
            cmd->topic_name(),
            cmd->payload());

      const auto msg = cmd->decode<demo::client_hello_t>();

      logger_->info("hello received. client={}, greeting={}",
            msg.id_, msg.greeting_);

      demo::publisher_t::publish(
            mqtt_,
            msg.reply_topic_,
            demo::server_hello_t{"World, hello!"});
   }
};

void launch_demo() {
   so_5::launch([&](so_5::environment_t & env) {
      auto loggers = demo::make_loggers();
      auto mqtt = demo::run_transport_manager(
            env, "demo-server", loggers.mqtt_logger_);

      env.introduce_coop([&](so_5::coop_t & coop) {
         coop.make_agent<listener_t>(mqtt, loggers.app_logger_);
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

