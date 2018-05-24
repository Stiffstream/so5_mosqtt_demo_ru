[TOC]

# Что это?
Это пример для иллюстрации статьи [Добавляем распределенность в SObjectizer-5 с помощью MQTT и libmosquitto](https://habr.com/post/359212/).

# Как взять и попробовать?
Для компиляции потребуется Linux, компилятор с поддержкой C++14 (gcc-5.4 или выше), Ruby+RubyGems+Rake.

Так же потребуется установленный в системе Boost (в mosquitto_transport используются Boost.Container и Boost.Algorithms). Остальные зависимости подтягиваются с помощью MxxRu.

## Компиляция с помощью MxxRu
Для компиляции с помощью MxxRu потребуется Ruby, RubyGems и Rake (как правило, RubyGems устанавливается вместе с Ruby, но если это не так, то RubyGems нужно поставить явно). Для установки MxxRu нужно выполнить команду:

~~~~~{.sh}
gem install Mxx_ru
~~~~~

Для установки Rake обычно нужно выполнить команду:
~~~~~{.sh}
gem install Rake
~~~~~
но это может зависеть от используемого вами Linux-дистрибутива.

После того как Ruby, RubyGems, Rake и MxxRu установлены можно брать примеры непосредственно из Hg-репозитория:

~~~~~{.sh}
hg clone https://eao197@bitbucket.org/sobjectizerteam/so5_mosqtt_demo_ru
cd so5_mosqtt_demo_ru
mxxruexternals
cd dev
ruby build.rb
~~~~~

Либо же можно загрузить архив с именем вида so5_mosqtt_demo_ru-*-full.zip из секции [Downloads](https://bitbucket.org/sobjectizerteam/so5_mosqtt_demo_ru/downloads/). После чего:

~~~~~{.sh}
unzip so5_mosqtt_demo_ru-201805241100-full.zip
cd so5_mosqtt_demo_ru/dev
ruby build.rb
~~~~~

Скомпилированные примеры должны оказаться внутри подкаталога `target/release`.

Также перед сборкой может быть полезно выполнить:

~~~~~{.sh}
cp local-build.rb.example local-build.rb
~~~~~

и нужным вам образом отредактировать содержимое `local-build.rb`.

## Примечание

Сборка с помошью CMake не поддерживается поскольку она не поддерживается в mosquitto_transport.
Если вы хотите иметь возможность собрать данный пример с помощью CMake, то заведите соответствующий Issue. В этом случае мы постараемся добавить поддержку CMake в mosquitto_transport, а затем и в данный пример.

