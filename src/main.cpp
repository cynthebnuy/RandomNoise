#include <Geode/Geode.hpp>
#include <random>

using namespace geode::prelude;

std::vector<std::string> sounds;

// thanks andy for the code <3 ily
int64_t randomInt(int64_t min, int64_t max) {
    static std::random_device device;
    static std::mt19937 generator(device());
    std::uniform_int_distribution<int64_t> distribution(min, max);

    return distribution(generator);
}

$on_mod(Loaded) {
	geode::log::info("Random Noise loaded!");

    auto mod = Mod::get();
    if (!mod->setSavedValue("first-load", true)) {
        std::error_code ec;
        std::filesystem::copy_file(mod->getResourcesDir() / "randsfx.ogg", mod->getConfigDir() / "randsfx.ogg", ec);
        if (ec) {
            geode::log::error("Failed to copy sound effect: {}", ec.message());
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(mod->getConfigDir())) {
        if (entry.is_regular_file()) sounds.push_back(string::pathToString(entry.path().filename()));
    }

    std::thread([] {
        while (true) {
            // geode::log::debug("Fat Tuesday is coming up");

            auto prob = Mod::get()->getSettingValue<int64_t>("probability");
            auto gen = randomInt(1, prob);
            // geode::log::debug("{}", gen);
            if (gen == 1) {
                // geode::log::debug("Activated!");

                geode::queueInMainThread([]() {
                    if (!sounds.empty()) {
                        auto sound = sounds[randomInt(0, sounds.size() - 1)];
                        auto path = Mod::get()->getConfigDir() / sound;
                        std::error_code ec;
                        if (std::filesystem::exists(path, ec)) {
                            FMODAudioEngine::get()->playEffect(string::pathToString(path));
                            return;
                        }
                        else {
                            sounds.erase(std::remove(sounds.begin(), sounds.end(), sound), sounds.end());
                        }
                    }

                    FMODAudioEngine::get()->playEffect("randsfx.ogg"_spr);
                });
            }

            std::this_thread::sleep_for(std::chrono::seconds{ 1 });
        }
    }).detach();
}