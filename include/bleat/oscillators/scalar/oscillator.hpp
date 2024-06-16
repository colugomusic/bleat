#pragma once

#include <functional>
#include "phasor.hpp"
    
namespace bleat {
namespace oscillators {
namespace scalar {

struct Oscillator {
	auto reset() -> void { phasor_.reset(); }
	auto value() const -> float { return value_; }
	auto sync_out() const -> float { return phasor_.sync_out(); }
protected:
	template <typename Fn> [[nodiscard]]	
	auto calculate(Fn calculator, float freq, float sync = -1.0f) -> float {
		return (value_ = calculator(phasor_(freq, sync)));
	}
private:
	Phasor phasor_;
	float value_ = 0.0f;
};

}}}