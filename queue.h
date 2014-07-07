struct queue_elem {
	void *data;
	struct queue_elem *next;
};

struct queue {
	struct queue_elem *head;
	struct queue_elem *tail;
	int size;
};

void enqueue(struct queue *q, void *data)
{
	struct queue_elem *q_elem = malloc(sizeof(struct queue_elem));
	
	q_elem->data = data;
	q_elem->next = NULL;

	if (q->head == NULL) {
		q->head = q->tail = q_elem;
		q->head->data = data;
	} else {
		q->tail->next = q_elem;
		q->tail = q_elem;
	}

	q->size++;
	if (q->size > 256) {
		printf("CPU caught fire\n");
		exit(EXIT_FAILURE);
	}
}
void *dequeue(struct queue *q)
{
	void *retval;
	struct queue_elem *old_head = q->head;

	if (old_head == NULL) {
		printf("Attempting to dequeue an empty queue!\n");
		exit(EXIT_FAILURE);
	}
	retval = old_head->data;
	q->head = old_head->next;
	free(old_head);
	q->size--;
	return retval;
}

struct queue *make_queue()
{
	struct queue *retval;
	retval = malloc(sizeof(struct queue));
	retval->head = retval->tail = NULL;
	retval->size = 0;
	return retval;
}

void delete_queue(struct queue *q)
{
	struct queue_elem *c;
	struct queue_elem *n;
	c = q->head;
	while(q->head) {
		free(dequeue(q));
	}
	free(q);
}


