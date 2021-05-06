#include "hashTable.h"

int main(int argc, char *argv[])
{   
    HASH_LEN = atoi(argv[1]);
    struct hashTable *map = create_hash(HASH_LEN);
    FILE *in = fopen(argv[2], "rt");
    out = fopen(argv[3], "w");

    char command[STRING_LEN] = {0};
    char key[STRING_LEN] = {0};
    char value[STRING_LEN] = {0};
    int index = 0;
    char *ret;
    while (fgets(command, STRING_LEN, in)) {
        command[strlen(command) - 1] = '\0';
        if (strncmp(command, "put", 3) == 0) {
            sscanf(command + 3, "%s %s", key, value);
            put(key, value, map);
        } else if (strncmp(command, "find", 4) == 0) {
            sscanf(command + 4, "%s", key);
            int rc = find(map, key);
            if (rc == -1)
                fprintf(out, "False\n");
            else
                fprintf(out, "True\n");
        } else if (strncmp(command, "print_bucket", 12) == 0) {
            sscanf(command + 12, "%d", &index);
            print_bucket(map, index);
        } else if (strncmp(command, "remove", 6) == 0) {
            sscanf(command + 6, "%s", key);
            remove_key(map ,key);
        } else if (strncmp(command, "get", 3) == 0) {
            sscanf(command + 3, "%s", key);
            ret = (char *)get(map, key);
            if (ret == NULL)
                fprintf(out, "NULL\n");
            else
                fprintf(out, "%s\n", ret);
        } else if (strncmp(command, "print", 5) == 0) {
            print(map);
        }
    }
    free_map(&map);
    fclose(in);
    fclose(out);
    return 0;
}