#include <stdlib.h>

#include "median.h"

// Comparison function for qsort
int compare(const void *a, const void *b)
{
    return (*(int32_t *)a - *(int32_t *)b);
}

// Initialize the median filter
void median_filter_init(median_filter_t *f, int32_t initial_value)
{
    f->index = 0;

    // Initialize the window to zeros
    for (int i = 0; i < CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE; i++)
    {
        f->window[i] = initial_value;
    }
}

// Update the median filter with a new measurement and get the filtered value
int32_t median_filter_update(median_filter_t *f, int32_t measurement)
{
    // Insert the new measurement into the window
    f->window[f->index] = measurement;
    f->index = (f->index + 1) % CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE;

    // Create a copy of the window for sorting
    int32_t sorted_window[CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE];
    for (int i = 0; i < CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE; i++)
    {
        sorted_window[i] = f->window[i];
    }

    // Sort the window to find the median
    qsort(sorted_window, CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE, sizeof(int32_t), compare);

    // Find the median value
    int32_t median;
    if (CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE % 2 == 0)
    {
        median = (sorted_window[CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE / 2 - 1] + sorted_window[CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE / 2]) / 2;
    }
    else
    {
        median = sorted_window[CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE / 2];
    }

    return median;
}