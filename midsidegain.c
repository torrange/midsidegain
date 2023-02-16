#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define MSGAIN_URI "http://lv2plug.in/plugins/midsidegain"


typedef enum {
	INPUT_L,
	INPUT_R,
   OUTPUT_L,
   OUTPUT_R,
   GAIN,
   GAIN_M,
   GAIN_S
} PortIndex;


typedef struct {
	float* input[2];
	float* output[2];
   float* gain;
   float* gain_m;
   float* gain_s;
} MidsideGain;


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
	MidsideGain* self = (MidsideGain*)malloc(sizeof(MidsideGain));

	return (LV2_Handle)self;
}


static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	MidsideGain* self = (MidsideGain*)instance;

	switch ((PortIndex)port) {
	case INPUT_L:
		self->input[0] = data;
		break;
	case INPUT_R:
		self->input[1] = data;
		break;
	case OUTPUT_L:
		self->output[0] = data;
		break;
	case OUTPUT_R:
		self->output[1] = data;
		break;
	case GAIN:
		self->gain = data;
		break;
   case GAIN_M:
		self->gain_m = data;
		break;
   case GAIN_S:
		self->gain_s = data;
		break;
	}
}


static void
activate(LV2_Handle instance)
{
}


/** Define a macro for converting a gain in dB to a coefficient. */
#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)


static void
run(LV2_Handle instance, uint32_t n_samples)
{
	const MidsideGain* self = (const MidsideGain*)instance;

	const float gain   = *(self->gain);
	const float coef_st = DB_CO(gain);

	const float gain_m   = *(self->gain_m);
	const float coef_m = DB_CO(gain_m);

	const float gain_s   = *(self->gain_s);
	const float coef_s = DB_CO(gain_s);

	for (uint32_t s = 0; s < n_samples; s++) {
      const float tmpM = coef_m * (.5 * (self->input[0][s] + self->input[1][s]));
      const float tmpS = coef_s * (.5 * (self->input[0][s] - self->input[1][s]));
      const float tmpL = tmpM + tmpS;
      const float tmpR = tmpM - tmpS;
		self->output[0][s] = coef_st * tmpL;
      self->output[1][s] = coef_st * tmpR;
	}
}


static void
deactivate(LV2_Handle instance)
{
}


static void
cleanup(LV2_Handle instance)
{
	free(instance);
}


static const void*
extension_data(const char* uri)
{
	return NULL;
}


static const LV2_Descriptor descriptor = {
	MSGAIN_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

/**
   The `lv2_descriptor()` function is the entry point to the plugin library.  The
   host will load the library and call this function repeatedly with increasing
   indices to find all the plugins defined in the library.  The index is not an
   indentifier, the URI of the returned descriptor is used to determine the
   identify of the plugin.

   This method is in the ``discovery'' threading class, so no other functions
   or methods in this plugin library will be called concurrently with it.
*/
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index) {
	case 0:  return &descriptor;
	default: return NULL;
	}
}
