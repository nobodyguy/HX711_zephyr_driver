#include "ema.h"

void ema_filter_init(ema_filter_t *f, int alpha)
{
    if (alpha < 0)
    {
        f->alpha = 0.0f;
    }
    else if (alpha > 100)
    {
        f->alpha = 1.0f;
    }
    else
    {
        f->alpha = (float)alpha / 100.0f;
    }

    f->out = 0.0;
}

int32_t ema_filter_update(ema_filter_t *f, int32_t measurement)
{
    f->out = f->alpha * (double)measurement + (1.0 - f->alpha) * f->out;
    return (int32_t)(f->out + 0.5);  // Round to nearest integer
}