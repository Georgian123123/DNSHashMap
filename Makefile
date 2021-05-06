build:
	gcc main.c hashTable.c CircularDoublyLinkedList.c hashTable.h CircularDoublyLinkedList.h -o tema1
clean:
	rm -rf CircularDoublyLinkedList.o hashTable.o