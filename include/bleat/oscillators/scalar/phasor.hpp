#pragma once

#include <cstdint>
#include "../../const_math.hpp"

namespace bleat {
namespace oscillators {
namespace scalar {

struct Phasor {
	static constexpr auto STEPS_PER_CYCLE = ::const_math::pow(2.0f, 32);
	static constexpr auto MIDPOINT        = STEPS_PER_CYCLE / 2;
	static constexpr auto CYCLES_PER_STEP = 1 / STEPS_PER_CYCLE;
	static constexpr auto MIDPOINT_INT    = static_cast<uint32_t>(MIDPOINT);
	auto reset() -> void {
		phase_ = 0;
	}
	auto sync_out() const -> float {
		return sync_out_;
	}
	auto operator()(float cycles_per_frame, float sync_in = -1.0f) -> float {
		const auto steps_per_sample     = cycles_per_frame * STEPS_PER_CYCLE;
		const auto inc                  = static_cast<uint32_t>(steps_per_sample);
		const auto prev_phase           = phase_;
		update_phase(inc, sync_in);
		update_sync_value(prev_phase, inc);
		return static_cast<float>(phase_) * CYCLES_PER_STEP;
	}
private:
	uint32_t phase_ = 0;
	float sync_out_ = -1.0f;
	auto update_phase(uint32_t inc, float sync_in) -> void {
		if (sync_in > 0.0f) {
			phase_ = MIDPOINT_INT + uint32_t(sync_in * inc);
		}
		else {
			phase_ += inc;
		}
	}
	auto update_sync_value(uint32_t prev_phase, uint32_t inc) -> void {
		if (prev_phase < MIDPOINT && phase_ >= MIDPOINT) {
			sync_out_ = float(phase_ - MIDPOINT) / inc;
			return;
		}
		sync_out_ = -1.0f;
	}
};

}}}

