#include <Geode/Geode.hpp>
#include <random>

using namespace geode::prelude;

// thanks andy for the code <3 ily
int64_t randomInt(int64_t min, int64_t max) {
    static std::random_device device;
    static std::mt19937 generator(device());
    std::uniform_int_distribution<int64_t> distribution(min, max);

    return distribution(generator);
}

$on_mod(Loaded) {
	geode::log::info("Random Noise loaded!");

    std::thread([] {
        while (true) {
            // geode::log::debug("Fat Tuesday is coming up");

            auto prob = Mod::get()->getSettingValue<int64_t>("probability");
            auto gen = randomInt(1, prob);
            // geode::log::debug("{}", gen);
            if (gen == 1) {
                // geode::log::debug("Activated!");

                FMODAudioEngine::get()->playEffect("randsfx.ogg"_spr);
            }

            std::this_thread::sleep_for(std::chrono::seconds{ 1 });
        }
    }).detach();
}