require 'rubygems'

gem 'Mxx_ru', '>= 1.3.0'

require 'mxx_ru/cpp'

MxxRu::Cpp::exe_target {

  target 'demo_server_app'

  required_prj 'mosquitto_transport/prj.rb'
  required_prj 'rapidjson_mxxru/prj.rb'

  cpp_source 'server.cpp'

}

