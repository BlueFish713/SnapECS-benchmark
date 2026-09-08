#ifndef ECS-BENCHMARK_VERSION_H
#define ECS-BENCHMARK_VERSION_H

#define CONFIG_PROJECT_NAME_STRING "ecs-benchmark"
#define CONFIG_PROJECT_VERSION_MAJOR 7
#define CONFIG_PROJECT_VERSION_MINOR 2
#define CONFIG_PROJECT_VERSION_PATCH 0
//#define CONFIG_PROJECT_VERSION_TWEAK 
#define CONFIG_PROJECT_VERSION_STRING "7.2.0"

struct Version {
	inline static constexpr const char* ProjectName = CONFIG_PROJECT_NAME_STRING;
	inline static constexpr int Manor = CONFIG_PROJECT_VERSION_MAJOR;
	inline static constexpr int Minor = CONFIG_PROJECT_VERSION_MINOR;
	inline static constexpr int Patch = CONFIG_PROJECT_VERSION_PATCH;
	//inline static constexpr int Tweak = PROJECT_VERSION_TWEAK;
	inline static constexpr const char* String = CONFIG_PROJECT_VERSION_STRING;
};

#endif
