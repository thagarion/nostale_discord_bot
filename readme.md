### Required libs

```shell
sudo apt-get install cmake gcc-13 build-essential
sudo apt-get install zlib1g-dev
sudo apt-get install libssl-dev
sudo apt-get install libyaml-cpp-dev
sudo apt-get install libtinyxml2-dev
sudo apt-get install libcurl4-openssl-dev
```

### Build

```shell
mkdir build && cd build
cmake -DBOT_TOKEN="\"discord.bot.token\"" -DGEMINI_TOKEN="\"gemini.token\"" .. && make
```

### Bot Settings

- Set News Channel `/conf news <channel.id>`
- Set Mara Channel `/conf mara <channel.id>`
- Create or Find Level roles `/conf roles`

### TODOs

- [ ] Valid stopping bot by signal 2 or 3
- [ ] Mara creation command
- [ ] Removing of messages in trading channel
- [x] Formatting news: split by image