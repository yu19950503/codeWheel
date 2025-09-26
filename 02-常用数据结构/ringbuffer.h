#include "stdint.h"
#include "string.h"

#define BUFFER_SIZE 1024

// Define the queue structure
struct queue_t
{
    uint16_t head;
    uint16_t tail;
    uint16_t used_len;
    uint16_t free_len;
    uint8_t buffer[BUFFER_SIZE];
};

// Initialize the queue
extern uint8_t queue_Init(struct queue_t *p_queue);

// Check if the queue is full
extern uint8_t queue_IsFull(struct queue_t *p_queue);

// Check if the queue is empty
extern uint8_t queue_IsEmpty(struct queue_t *p_queue);

// Get the number of used bytes in the queue
extern uint16_t queue_used_size(struct queue_t *p_queue);

// Get the number of free bytes in the queue
extern uint16_t queue_free_size(struct queue_t *p_queue);

// Write data to the queue
extern uint8_t queue_write(struct queue_t *p_queue, const uint8_t *p_items, uint16_t items_size);

// Read data from the queue
extern uint8_t queue_read(struct queue_t *p_queue, uint8_t *p_items, uint16_t items_size);
