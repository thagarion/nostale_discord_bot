#include <gtest/gtest.h>

#include "rss_feed/rss_event.hpp"

TEST(RSSEvent, set_content_short) {
    std::string message = R""""("## Great news, everyone!

https://image.board.gameforge.com/uploads/nostale/en/announcement_nostale_en_e26ff8ebdffe8793801d1453997fc980.jpg

The NosWheel has lots of new rewards that can help you on your adventures. Spin the wheel today and find out what surprise is waiting for you!

Even if you’re already a NosWheel veteran, it’s worth a try because Fortuna’s breath of fresh air has brought brand-new prizes.

### Potential Prizes

* 10x Sealed Champion Blessing Amulet (Random)
* Retro Wings Box
* Baby Panda
* Gingerbread Fortune Box
* Vampire Marie
* 20x Fairy Booster
* Pet Bead
* One-Winged Perti Specialist Partner Card
* 20x Large Lump of Gold + 500 NosDollars
* Fabulous (Title)
* ... and much more!

Remember you’ve got to spin it to win it!

The NosTale Team
")"""";

    auto event = RSSEvent();
    event.set_content(message);

    const auto messages = event.get_content();
    EXPECT_EQ(messages->size(), 1);
}

TEST(RSSEvent, set_content) {
    std::string message = R""""(
With great effort together with Mimi and Eva you have managed to gather some clues by completing tasks and solving puzzles. Now, we are closer to our goal than ever before!.

But the evil forces seem to still persist! evil creatures have taken control of the seasons and Mimi has to do something to save Nosville from its imminent doom.

Your help is needed for she can't do it alone! with the power of the magical necklace you found, it might be possible. Give your aid to Mimi to save our beloved town!

Join us and Mimi on a journey through the four seasons to eliminate the monster waves.

To show you her gratitude for helping, Mimi has some nice rewards for you at the end.

- When and Where -

Gather on Channel 7 to find out what’s happening and to put the minions in their place:

27.10.2024 at 15:00 CETS

https://s20.directupload.net/images/240928/5c4esttr.png

The clues will be shown to you through a speech bubble, but first, you have to find us in this seasonal chaos...

As you start your adventure in search of the clues you need to find, once you find all of them you need to solve them and name all 4 words, so keep them safe!

After you have defeated all the villains from the different seasons, you must whisper your solution to one of the clue givers at the very end, but you only have 15 minutes to do so.

Only whisper submissions from Channel 7 will be accepted.

https://s20.directupload.net/images/240928/4egezyww.png)"""";

    auto event = RSSEvent();
    event.set_content(message);

    const auto messages = event.get_content();
    EXPECT_EQ(messages->size(), 2);
}

TEST(RSSEvent, set_content_long) {
    std::string message = R""""(
With great effort together with Mimi and Eva you have managed to gather some clues by completing tasks and solving puzzles. Now, we are closer to our goal than ever before!.

But the evil forces seem to still persist! evil creatures have taken control of the seasons and Mimi has to do something to save Nosville from its imminent doom.

Your help is needed for she can't do it alone! with the power of the magical necklace you found, it might be possible. Give your aid to Mimi to save our beloved town!

Join us and Mimi on a journey through the four seasons to eliminate the monster waves.

To show you her gratitude for helping, Mimi has some nice rewards for you at the end.

- When and Where -

Gather on Channel 7 to find out what’s happening and to put the minions in their place:

27.10.2024 at 15:00 CETS

With great effort together with Mimi and Eva you have managed to gather some clues by completing tasks and solving puzzles. Now, we are closer to our goal than ever before!.

But the evil forces seem to still persist! evil creatures have taken control of the seasons and Mimi has to do something to save Nosville from its imminent doom.

Your help is needed for she can't do it alone! with the power of the magical necklace you found, it might be possible. Give your aid to Mimi to save our beloved town!

Join us and Mimi on a journey through the four seasons to eliminate the monster waves.

To show you her gratitude for helping, Mimi has some nice rewards for you at the end.

- When and Where -

Gather on Channel 7 to find out what’s happening and to put the minions in their place:

27.10.2024 at 15:00 CETS

With great effort together with Mimi and Eva you have managed to gather some clues by completing tasks and solving puzzles. Now, we are closer to our goal than ever before!.

But the evil forces seem to still persist! evil creatures have taken control of the seasons and Mimi has to do something to save Nosville from its imminent doom.

Your help is needed for she can't do it alone! with the power of the magical necklace you found, it might be possible. Give your aid to Mimi to save our beloved town!

Join us and Mimi on a journey through the four seasons to eliminate the monster waves.

To show you her gratitude for helping, Mimi has some nice rewards for you at the end.

https://s20.directupload.net/images/240928/5c4esttr.png

The clues will be shown to you through a speech bubble, but first, you have to find us in this seasonal chaos...

As you start your adventure in search of the clues you need to find, once you find all of them you need to solve them and name all 4 words, so keep them safe!

After you have defeated all the villains from the different seasons, you must whisper your solution to one of the clue givers at the very end, but you only have 15 minutes to do so.

Only whisper submissions from Channel 7 will be accepted.

https://s20.directupload.net/images/240928/4egezyww.png)"""";

    auto event = RSSEvent();
    event.set_content(message);

    const auto messages = event.get_content();
    EXPECT_EQ(messages->size(), 3);
}