# Basic CI/CD

Разработка простого **CI/CD** для проекта *SimpleBashUtils*. Сборка, тестирование, развертывание.

<br>

## Содержание

1. [Настройка gitlab-runner](#part-1-настройка-gitlab-runner) \
  1.1. [Установка виртуальной машины](#поднять-виртуальную-машину-ubuntu-server-2204-lts) \
  1.2. [Установка gitlab-runner](#скачать-и-установить-на-виртуальную-машину-gitlab-runner) \
  1.3. [Регистрация в gitlab-runner](#запустить-gitlab-runner-и-зарегистрировать-его-для-использования-в-текущем-проекте-do6_cicd)
2. [Cборка](#part-2-сборка) \
  2.1. [Добавление этапа сборки](#напиcать-этап-для-ci-по-сборке-приложений-из-проекта-c2_simplebashutils) \
  2.2. [Неподготовленная оболочка](#неподготовленная-оболочка) \
  2.3. [Проверка сборки проекта](#проверка-сборки-проекта)
3. [Тест кодстайла](#part-3-тест-кодстайла) \
  3.1. [Добавление этапа теста кодстайла](#напиcать-этап-для-ci-который-запускает-скрипт-кодстайла-clang-format) \
  3.2. [Ошибочный вывод пайплайна](#проверить-зафейлился-ли-пайплайн-если-совершена-ошибка-в-кодстайле) \
  3.3. [Корректный вывод пайплайна](#исправить-ошибку-в-форматировании-кода-и-проверить-результат)
4. [Интеграционные тесты](#part-4-интеграционные-тесты) \
  4.1. [Добавление этапа интеграционных тестов](#написать-этап-для-ci-который-запускает-интеграционные-тесты-из-того-же-проекта) \
  4.2. [Ошибочный вывод пайплайна](#проверить-зафейлился-ли-пайплайн-если-обнаруживаются-ошибочные-рузльтаты-интеграционных-тестов) \
  4.3. [Корректный вывод пайплайна](#исправить-ошибку-для-успешного-прохождения-тестов-и-проверить-результат)
5. [Этап деплоя](#part-5-этап-деплоя) \
  5.1. [Установка виртуальной машины](#поднять-вторую-виртуальную-машину-ubuntu-server-2204-lts) \
  5.2. [Статическая маршрутизация между двумя машинами](#статическая-маршрутизация-между-двумя-машинами) \
  5.3. [Генерация ssh-ключей](#генерация-ssh-ключей) \
  5.4. [Относительно bash-скрипта](#написать-bash-скрипт-который-при-помощи-ssh-и-scp-копирует-файлы-полученные-после-сборки-артефакты-в-директорию-usrlocalbin-второй-виртуальной-машины) \
  5.5. [Добавление этапа деплоя](#написать-этап-для-cd-который-«разворачивает»-проект-на-другой-виртуальной-машине) \
  5.6. [Настройка ssh-агента](#настройка-ssh-агента) \
  5.7. [Проверка этапа деплоя](#проверка-этапа-деплоя)
6. [Дополнительно. Уведомления](#part-6-дополнительно-уведомления) \
  6.1. [Создание бота и получение данных](#настроить-уведомления-о-успешномнеуспешном-выполнении-пайплайна-через-бота-с-именем-«nealpadm-do6-cicd»-в-telegram) \
  6.2. [Настройка бота](#настройка-бота) \
  6.3. [Проверка работы бота](#проверка-работы-бота)

<br>




## [Part 1. Настройка **gitlab-runner**](#содержание)

**== Задание ==**

### Поднять виртуальную машину *Ubuntu Server 22.04 LTS*

![1.1](screenshots/1.png) 

> В дальнейшем работа на Ubutu Server будет производиться через openssh на декстопной версии Ubuntu

### Скачать и установить на виртуальную машину **gitlab-runner**

> Был выбран метод установки gitlab-runner через [бинарный файл с официального сайта](https://docs.gitlab.com/runner/install/linux-manually.html)

1. Загрузить бинарный файл <br>
```sh
sudo curl -L --output /usr/local/bin/gitlab-runner "https://s3.dualstack.us-east-1.amazonaws.com/gitlab-runner-downloads/latest/binaries/gitlab-runner-linux-amd64"
```
2. Дать файлу разрешение на исполнение: <br>
```sh
sudo chmod +x /usr/local/bin/gitlab-runner
```
3. Создать пользователя GitLab CI <br>
```sh
sudo useradd --comment 'GitLab Runner' --create-home gitlab-runner --shell /bin/bash
```
4. Установить как службу <br>
```sh
sudo gitlab-runner install --user=gitlab-runner --working-directory=/home/gitlab-runner
```
5. Запустить службу
```sh
sudo gitlab-runner start
```

![1.2](screenshots/2.png) 

### Запустить **gitlab-runner** и зарегистрировать его для использования в текущем проекте (*DO6_CICD*)

> Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе

![1.3](screenshots/3.png) 

- Зарегистрировать gitlab-runner
```ssh
sudo gitlab-runner register
```
> Для этого необходимо ввести данные при регистрации: <br>
> 1. Cвой URL-адрес GitLab
> 2. Cвой регистрационный токен
> 3. Название раннера
> 4. Теги для заданий, разделенные запятыми
> 5. Тип исполнителя

![1.4](screenshots/4.png) 

<br>




## [Part 2. Сборка](#содержание)

### Напиcать этап для CI по сборке приложений из проекта *C2_SimpleBashUtils*

- В корне репозитория создать файл `.gitlab-ci.yml`

```ssh
touch .gitlab-ci.yml
```

- Добавить в файл этап запуска сборки через мейк файл из проекта C2.

> Настроим также этап, чтобы файлы, полученные после сборки (артефакты), сохранялись со сроком хранения 30 дней.

![2.1](screenshots/5.png)

### Неподготовленная оболочка

- При пуше мы столкнемся со следующей ошибкой

![2.2](screenshots/6.png)

> Раннер нас предупреждает, что среда не подготовлена к запуску. Причиной послужила дефолтная конфигурация gitlab-runner, производящая очистку терминала при выходе из оболочки shell. Комментирование строк данного скрипта устраняет данную ошибку

- Закомментируем строки в `/home/gitlab-runner/.bash_logout`

![2.3](screenshots/7.png)

### Проверка сборки проекта

- Перезапустим пайплайн и проверим пропала ли ошибка

![2.4](screenshots/8.png)

- Как можно увидеть, сборка была успешно осуществлена, исполняемые файлы были сохранены на 30 дней

![2.5](screenshots/9.png)

<br>




## [Part 3. Тест кодстайла](#содержание)

### Напиcать этап для CI, который запускает скрипт кодстайла (clang-format)

![3.1](screenshots/10.png)

### Проверить зафейлился ли пайплайн, если совершена ошибка в кодстайле

- Проверим сначала вывод команды локально

![3.2](screenshots/11.png)

- Как и ожидалось - пайплайн зафейлился

![3.3](screenshots/12.png)

- Вывод пайплайна совпал с локальным выводом команды

![3.4](screenshots/13.png)

### Исправить ошибку в форматировании кода и проверить результат

- Результат работы пайплайна

![3.5](screenshots/14.png)

- Теперь проект успешно проходит тест кодстайла

![3.6](screenshots/15.png)

<br>




## [Part 4. Интеграционные тесты](#содержание)

### Написать этап для CI, который запускает интеграционные тесты из того же проекта

![4.1](screenshots/16.png)

### Проверить зафейлился ли пайплайн, если обнаруживаются ошибочные рузльтаты интеграционных тестов

- Проверим сначала вывод интеграционных тестов локально

![4.2](screenshots/17.png)

- Проверяем, что пайплайн зафейлился

![4.3](screenshots/18.png)

- Вывод пайплайна совпал с локальным выводом результатов

![4.4](screenshots/19.png)

### Исправить ошибку для успешного прохождения тестов и проверить результат

- Результат работы пайплайна

![4.5](screenshots/20.png)

- Проект успешно проходит интеграционные тесты

![4.6](screenshots/21.png)

<br>




## [Part 5. Этап деплоя](#содержание)

### Поднять вторую виртуальную машину Ubuntu Server 22.04 LTS

![5.1](screenshots/22.png)

### Статическая маршрутизация между двумя машинами

- Настроим адаптеры обоих машин на внутреннюю сеть

> Для удобства раннеровская машина находится в сети с десктопной версией Ubuntu. В дальнейшем консоли обеих машин будут отображаться на десктопной версии

```sh
sudo vim /etc/netplan/00-network-manager-all.yaml
```

![5.2](screenshots/23.png)

- Обязательно принимаем изменения в настройках адаптеров

```sh
sudo netplan apply
```

- Проверим соединение между машинами

![5.3](screenshots/24.png)

### Генерация ssh-ключей

- Для начала сгенерируем пары ключей для каждой машины

```sh
ssh-keygen
```
![5.4](screenshots/25.png)

- Добавим открытый ключ второй машины с вывода `cat /home/nealpadm/.ssh/id_rsa.pub` в ssh ключи gitlab для работы с проектом на удаленной машине

![5.5](screenshots/26.png)

### Написать bash-скрипт, который при помощи ssh и scp копирует файлы, полученные после сборки (артефакты), в директорию /usr/local/bin второй виртуальной машины

> Данная часть предполагает создание отдельного скрипта и применение его в стейдже. Так как ранее все скрипты были описаны в `.gitlab-ci.yml`, то, пожалуй, сохраним данную тенденцию, дабы не засорять репозиторий ничем иным. <br>
> Технически команды yml исполняет bash, поэтому будем считать, что "был написан bash-скрипт". <br>

### Написать этап для CD, который «разворачивает» проект на другой виртуальной машине

> Данный этап будет запускаться вручную при условии, что все предыдущие этапы прошли успешно

- Теперь, когда на удаленной машине есть проект, допишем этап деплоя в `.gitlab-ci.yml`

![5.6](screenshots/27.png)

> Этап деплоя описан таким образом, что для получения доступа к удаленному серверу применяется ssh-агент. Для его работы необходимо будет выполнить определенный ряд действий

### Настройка ssh-агента

- Следующие изменения будут происходить от суперпользователя, поэтому сразу же перейдем в этот режим на машине с развернутым раннером

```sh
sudo su
```

- Перейдем в настройки раннера и обозначим ему где искать ssh-агента. Для этого в файле конфигураций добавим строку `environment = ["SSH_AUTH_SOCK=/tmp/ssh-agent"]`

```sh
vim /etc/gitlab-runner/config.toml
```
![5.7](screenshots/28.png)

- Далее необходимо сохранить отпечаток удаленного сервера. Для этого ключ удаленного сервера необходимо добавить в файл `known_hosts` в домашнем каталоге пользователя gitlab-runner

```sh
ssh-keyscan -H 192.10.10.2 >> /home/gitlab-runner/.ssh/known_hosts
```
![5.8](screenshots/29.png)

- Так как раннеру необходим приватный ключ, для того, чтобы не возникало проблем с ключами доступа к директории `/home/nealpadm/.ssh` и ее содержимому - скопируем закрытый ключ также в домашний каталог пользователя gitlab-runner

```sh
cp /home/nealpadm/.ssh/id_rsa /home/gitlab-runner/.ssh/
```
![5.9](screenshots/30.png)

- В домашнем каталоге раннера есть все необходимое, однако на скрине можно заметить - так как мы находимся в режиме суперпользователя, в ключах указан root. Для того, чтобы раннер сумел их счесть, поменяем пользователя на gitlab-runner

```sh
chown gitlab-runner:gitlab-runner id_rsa known_hosts
```
![5.10](screenshots/31.png)

- Теперь с ключами доступа не должно возникнуть каких-либо проблем, дело осталось за малым. Режим суперпользователя нам более не необходим. Покидаем его зажатием комбинации Ctrl + D

![5.11](screenshots/32.png)

- Скопируем содержимое нашего открытого ssh-ключа на удаленный сервер

> Отвечаем `yes` на подтверждение подключения и в следующем действии вводим пароль от удаленной машины

```sh
ssh-copy-id nealpadm@192.10.10.2
```
![5.12](screenshots/33.png)

- После успешной настройки ssh-агента есть один небольшой штришок. Так как наш скрипт производит копирование в директорию, в которой у нас нет прав - даем право пользователю (в нашем случае `nealpadm`) на проведение команды `mv` (по скрипту необходима команда `mv`) с правами суперпользователя без ввода пароля. Для этого на удаленной машине в файле `/etc/sudoers` допишем строку `nealpadm ALL=(ALL) NOPASSWD: /bin/mv`

```sh
sudo visudo
```
![5.13](screenshots/34.png)

- Теперь можно с уверенностью сказать, что этап деплоя не выдаст ошибок. Перезапустим службу раннера и тем самым закончим настройку ssh-агента

```sh
sudo systemctl restart gitlab-runner.service
```

### Проверка этапа деплоя

- После пуша обновленного `gitlab-ci.yml` проверяем состояние пайплайна

![5.14](screenshots/35.png)

- Как видим пайплайн остановился после CI стадий и ждет нашей команды. Запустим этап деплоя

![5.15](screenshots/36.png)

- Судя по выводу раннера деплой прошел успешно. Проверим наличие полученных исполняемых файлов в директории `/usr/local/bin` на удаленной машине

![5.16](screenshots/37.png)

- Отлично, стадия деплоя отработает как положено. Стоит проверить и случай, когда пайплайн зафейлится. Для этого, пожалуй, уберем стадии `build` и `test`, чтобы ему было нечего копировать

![5.17](screenshots/38.png)

> Разумеется есть четкое понимание, что если стадия `build` зафейлится, то до деплоя, конечно же, не дойдет. Но проверить работу ветвления все же стоило, ибо в следующий раз это может быть не копирование артефакта, а чего-либо другого

<br>




## [Part 6. Дополнительно. Уведомления](#содержание)

### Настроить уведомления о успешном/неуспешном выполнении пайплайна через бота с именем «nealpadm DO6 CI/CD» в Telegram

> Текст уведомления будет содержать информацию об успешности прохождения как этапа CI, так и этапа CD. <br>
> В остальном текст уведомления может быть произвольным.

- Найдем в телеграме через поиск `BotFather`

![6.1](screenshots/39.png)

- Запустим бота и напишем `/newbot`

> В диалоге необходимо будет написать: <br>
> - имя бота `nealpadm DO6 CI/CD» в Telegram` <br>
> - юзернейм для бота (имя должно быть уникальным и заканчиваться на `bot`)

![6.2](screenshots/40.png)

- В результате мы получили `API` бота. Теперь найдем бота `getmyid_bot` и напишем ему `/start` для получения нашего `ID`

![6.3](screenshots/41.png)

> В данном задании мы продолжим тенденцию "незасорения репозитория", поэтому опишем notify полностью внутри `gitlab-ci.yml`

- Для того, чтобы не светить нашими API и ID, опишем все ключевые переменные для обращения к удаленному серверу (в этом случае это теперь бот) в конфигурационном файле `config.conf`, который мы создадим в корне раннеровской машины. Его содержимое будет следующим

```sh
vim ~/config.conf
```
![6.4](screenshots/42.png)

### Настройка бота

- Теперь перейдем непосредственно к этапам раннера и внесем некоторые коррективы

> Через `.notify: &notify` определим набор команд для уведомлений, которые будут использованы во всех этапах
> - Подключаем переменные из `config.conf`
> - Формируем сообщение боту, исходя из результата предыдущего job'а
> - Отправляем сформированное сообщение боту с помощью curl и ssh
> - У каждого стейджа в `after_scrict` указываем вызов вышеупомянутого набора команд

![6.5](screenshots/43.png)

### Проверка работы бота

- Протестируем уведомления от всех успешных и каждого зафейленного пайплайна

![6.4](screenshots/44.png)

### [К содержанию](#содержание)