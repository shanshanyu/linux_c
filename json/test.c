#include <stdio.h>
#include "cJSON.h"

int main() {
    char *json_string = "{\"name\":\"Tom\",\"age\":25}";
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        printf("Failed to parse JSON.\n");
        return 1;
    }
    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *age = cJSON_GetObjectItem(json, "age");
    printf("name: %s\n", name->valuestring);
    printf("age: %d\n", age->valueint);
    cJSON_Delete(json);
    return 0;
}
