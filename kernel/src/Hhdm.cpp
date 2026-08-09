#include "Hhdm.hpp"

#include <limine.h>

__attribute__((used, aligned(8)))
static volatile struct limine_hhdm_request hhdmRequest = {
	.id = LIMINE_HHDM_REQUEST_ID,
	.revision = 4,
	.response = nullptr
};

void Hhdm::init() {
	offset = hhdmRequest.response->offset;
}
