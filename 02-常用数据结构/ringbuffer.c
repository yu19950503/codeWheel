#include "stdint.h"
#include "string.h"
#include "ringbuffer.h"

// Initialize the queue
uint8_t queue_Init(struct queue_t *p_queue)
{
    if(p_queue == NULL)
    {
        return 0;
    }

    p_queue->head = p_queue->tail = 0;

    return 1;
}

// Check if the queue is full
uint8_t queue_IsFull(struct queue_t *p_queue)
{
    if(p_queue == NULL)
    {
        return 0;
    }

    return (((p_queue->head + 1) % BUFFER_SIZE == p_queue->tail) ? 1 : 0);
}

// Check if the queue is empty
uint8_t queue_IsEmpty(struct queue_t *p_queue)
{
    if(p_queue == NULL)
    {
        return 0;
    }

    return (p_queue->head == p_queue->tail ? 1 : 0);
}

// Get the number of used bytes in the queue
uint16_t queue_used_size(struct queue_t *p_queue)
{
    if(p_queue == NULL)
    {
        return 0;
    }

    return (p_queue->head >= p_queue->tail ? p_queue->head - p_queue->tail : (BUFFER_SIZE - p_queue->tail + p_queue->head));
}

// Get the number of free bytes in the queue
uint16_t queue_free_size(struct queue_t *p_queue)
{
    if(p_queue == NULL)
    {
        return 0;
    }

    return (BUFFER_SIZE - queue_used_size(p_queue) - 1);
}

// Write data to the queue
uint8_t queue_write(struct queue_t *p_queue, const uint8_t *p_items, uint16_t items_size)
{
    if(p_queue == NULL || p_items == NULL)
    {
        return 0;
    }

    if(!queue_IsFull(p_queue))
    {
        memcpy(&p_queue->buffer[p_queue->head], p_items, items_size);
        p_queue->head += items_size;

        return 1;
    }
    else
    {
        return 0;
    }
}

// Read data from the queue
uint8_t queue_read(struct queue_t *p_queue, uint8_t *p_items, uint16_t items_size)
{
    if(p_queue == NULL || p_items == NULL)
    {
        return 0;
    }

    if(!queue_IsEmpty(p_queue))
    {
        memcpy(p_items, &p_queue->buffer[p_queue->tail], items_size);
        p_queue->tail += items_size;

        return 1;
    }
    else
    {
        return 0;
    }
}