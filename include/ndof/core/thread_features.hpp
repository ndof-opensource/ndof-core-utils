

#ifndef NDOF_CORE_CONFIGURABLE_THREAD_GUARDS_HPP
#define NDOF_CORE_CONFIGURABLE_THREAD_GUARDS_HPP

#include <mutex>
#include <shared_mutex>

#include "configs.hpp"

namespace ndof {

/// A mutex implementation for configurations in which threading is disabled.
/// Its interface intentionally matches std::mutex, allowing the same guard
/// code to be used in threaded and single-threaded builds.
class no_thread_mutex {
public:
	no_thread_mutex() = default;
	~no_thread_mutex() = default;
	no_thread_mutex(const no_thread_mutex&) = delete;
	no_thread_mutex& operator=(const no_thread_mutex&) = delete;
	no_thread_mutex(no_thread_mutex&&) = delete;
	no_thread_mutex& operator=(no_thread_mutex&&) = delete;

	void lock() noexcept {}
	bool try_lock() noexcept { return true; }
	void unlock() noexcept {}
};

/// A no-op counterpart of std::shared_mutex.
class no_thread_shared_mutex {
public:
	no_thread_shared_mutex() = default;
	~no_thread_shared_mutex() = default;
	no_thread_shared_mutex(const no_thread_shared_mutex&) = delete;
	no_thread_shared_mutex& operator=(const no_thread_shared_mutex&) = delete;
	no_thread_shared_mutex(no_thread_shared_mutex&&) = delete;
	no_thread_shared_mutex& operator=(no_thread_shared_mutex&&) = delete;

	void lock() noexcept {}
	bool try_lock() noexcept { return true; }
	void unlock() noexcept {}
	void lock_shared() noexcept {}
	bool try_lock_shared() noexcept { return true; }
	void unlock_shared() noexcept {}
};

namespace detail {

template <bool ThreadsAvailable>
struct configurable_mutex_selector;

template <bool ThreadsAvailable>
	requires (ThreadsAvailable && ::ndof::threads_feature_enabled())
struct configurable_mutex_selector<ThreadsAvailable> {
	using type = std::mutex;
};

template <bool ThreadsAvailable>
	requires (!ThreadsAvailable || !::ndof::threads_feature_enabled())
struct configurable_mutex_selector<ThreadsAvailable> {
	using type = no_thread_mutex;
};

template <bool ThreadsAvailable>
struct configurable_shared_mutex_selector;

template <bool ThreadsAvailable>
	requires (ThreadsAvailable && ::ndof::threads_feature_enabled())
struct configurable_shared_mutex_selector<ThreadsAvailable> {
	using type = std::shared_mutex;
};

template <bool ThreadsAvailable>
	requires (!ThreadsAvailable || !::ndof::threads_feature_enabled())
struct configurable_shared_mutex_selector<ThreadsAvailable> {
	using type = no_thread_shared_mutex;
};

} // namespace detail

/// Selects a real mutex only when threads are requested and available.
template <bool ThreadsAvailable>
using configurable_mutex = typename detail::configurable_mutex_selector<ThreadsAvailable>::type;

/// Selects a real shared mutex only when threads are requested and available.
template <bool ThreadsAvailable>
using configurable_shared_mutex =
	typename detail::configurable_shared_mutex_selector<ThreadsAvailable>::type;

template <bool ThreadsAvailable>
using configurable_thread_guard = std::lock_guard<configurable_mutex<ThreadsAvailable>>;

template <bool ThreadsAvailable>
using configurable_unique_thread_guard = std::unique_lock<configurable_mutex<ThreadsAvailable>>;

template <bool ThreadsAvailable>
using configurable_shared_thread_guard =
	std::shared_lock<configurable_shared_mutex<ThreadsAvailable>>;

template <bool ThreadsAvailable>
using configurable_unique_shared_thread_guard =
	std::unique_lock<configurable_shared_mutex<ThreadsAvailable>>;

/// Creates a guard which locks only when ThreadsAvailable is true.
template <bool ThreadsAvailable>
inline configurable_thread_guard<ThreadsAvailable> make_thread_guard(
	configurable_mutex<ThreadsAvailable>& mutex) {
	return configurable_thread_guard<ThreadsAvailable>(mutex);
}

/// Creates a movable exclusive guard which locks only when ThreadsAvailable is true.
template <bool ThreadsAvailable>
inline configurable_unique_thread_guard<ThreadsAvailable> make_unique_thread_guard(
	configurable_mutex<ThreadsAvailable>& mutex) {
	return configurable_unique_thread_guard<ThreadsAvailable>(mutex);
}

/// Creates a shared guard which locks only when ThreadsAvailable is true.
template <bool ThreadsAvailable>
inline configurable_shared_thread_guard<ThreadsAvailable> make_shared_thread_guard(
	configurable_shared_mutex<ThreadsAvailable>& mutex) {
	return configurable_shared_thread_guard<ThreadsAvailable>(mutex);
}

/// Creates a movable exclusive shared-mutex guard which locks only when threads are available.
template <bool ThreadsAvailable>
inline configurable_unique_shared_thread_guard<ThreadsAvailable>
make_unique_shared_thread_guard(configurable_shared_mutex<ThreadsAvailable>& mutex) {
	return configurable_unique_shared_thread_guard<ThreadsAvailable>(mutex);
}

} // namespace ndof

#endif // NDOF_CORE_CONFIGURABLE_THREAD_GUARDS_HPP
