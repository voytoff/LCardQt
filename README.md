# LCard Library
### Установка модулей LCard на OS Linux DEB
= Из описания =
* Добавление внешнего источника:
```bash
echo ’deb http://download.opensuse.org/repositories/home:/l-card/xUbuntu_25.04/ /’ |
sudo tee /etc/apt/sources.list.d/home:l-card.list
```
* Скачивание и установка ключа подписи:
```bash
curl -fsSL
https://download.opensuse.org/repositories/home:l-card/xUbuntu_25.04/Release.key | gpg
–dearmor | sudo tee /etc/apt/trusted.gpg.d/home_l-card.gpg > /dev/null
```
Для других дистрибутивов достаточно в инстуркции **Debian_10** заменить на соответствующее название и версию дистрибутива, например для **Ubunut 25.04** на **xUbuntu_25.04**.
Далее достаточно просто обновить список пакетов и установить требуемые. Это можно сделать либо из графической оболочки, либо через командную строку. Например, если устанавливается 3 пакета, **ltrd, ltrmanger и libltrapi1-dev**, то вызов выглядит следующим образом:
```bash
sudo apt-get update
sudo apt-get install ltrd ltrmanger libltrapi1-dev
```
https://lumpics.ru/how-to-install-tar-gz-in-ubuntu/

### В результате все свелось к коду:
```bash
echo 'deb http://download.opensuse.org/repositories/home:/l-card/xUbuntu_24.04/ /' | sudo tee /etc/apt/sources.list.d/home:l-card.list
curl -fsSL https://download.opensuse.org/repositories/home:l-card/xUbuntu_24.04/Release.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/home_l-card.gpg > /dev/null
sudo apt-get update
sudo apt-get install ltrd ltrmanager libltrapi1-dev
sudo apt-get install ltrapi
```

-----------------
http://mirror.aardsoft.fi/opensuse/repositories/home%3A/l-card/xUbuntu_24.04

## Ручная установка пакетов:
Если по какой-либо причине добавлять внешний источник пакетов не удобно, то нужно скачать
требуемые пакеты и установить их вручную. При этом нужно будет установить и все требуемые зависимости.
Скачать пакеты можно по ссылке http://download.opensuse.org/repositories/home:/l-card/-
<дистрибутив>. Зависимые от архитектуры пакеты лежат в поддиректориях с названием архитектуры
(i386, i586 или i686 для 32-битной версии, x86_64 для 64-битной). Не зависящие от архитектуры
(например lpcie-dkms, так как он содержит исходники, а не уже скомпилированный под конкретную
архитектуру модуль) в поддиректории all или noarch
Предполагая, что все нужные пакеты скачаны и сохранены в директорию, где нет других файлов с тем же расширением, установку пакетов можно выполнить следующей командой из консоли из
директории со скачанными пакетами:
* Для дистрибутивов с пакетами формата .deb:
```bash
sudo dpkg -i *.deb
```
* Для дистрибутивов с пакетами формата .rpm:
```bash
sudo rpm -i *.rpm
```
* Для дистрибутива Arch Linux:
```bash
sudo pacman -U *.xz
```