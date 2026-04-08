#include "snekobject.h"
#include <stdlib.h>
#include <string.h>


void refcount_inc(snek_object_t *obj) {
    if (obj != NULL) {
        obj->refcount++;
    }
}

void refcount_dec(snek_object_t *obj) {
    if (obj != NULL && obj->refcount > 0) {
        obj->refcount--;
    }
    if (obj != NULL && obj->refcount == 0) {
        // Free the object based on its kind
        switch (obj->kind) {
            case INTEGER:
                break;
            case FLOAT:
                // No additional memory to free
                break;

            case STRING:
                free(obj->data.v_string);
                break;
        //     case VECTOR3:
        //         refcount_dec(obj->data.v_vector3.x);
        //         refcount_dec(obj->data.v_vector3.y);
        //         refcount_dec(obj->data.v_vector3.z);
        //         break;
        //     case ARRAY:
        //         for (size_t i = 0; i < obj->data.v_array.size; i++) {
        //             refcount_dec(obj->data.v_array.elements[i]);
        //         }
        //         free(obj->data.v_array.elements);
        //         break;
        // }
        free(obj);
    }
}

snek_object_t *_new_snek_object() {
  snek_object_t *obj = calloc(1, sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->refcount = 1;
  return obj;
}

// don't touch below this line

snek_object_t *new_snek_array(size_t size) {
  snek_object_t *obj = _new_snek_object();
  if (obj == NULL) {
    return NULL;
  }

  snek_object_t **elements = calloc(size, sizeof(snek_object_t *));
  if (elements == NULL) {
    free(obj);
    return NULL;
  }

  obj->kind = ARRAY;
  obj->data.v_array = (snek_array_t){.size = size, .elements = elements};

  return obj;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y,
                                snek_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  snek_object_t *obj = _new_snek_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR3;
  obj->data.v_vector3 = (snek_vector_t){.x = x, .y = y, .z = z};

  return obj;
}

snek_object_t *new_snek_integer(int value) {
  snek_object_t *obj = _new_snek_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;
  return obj;
}

snek_object_t *new_snek_float(float value) {
  snek_object_t *obj = _new_snek_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

snek_object_t *new_snek_string(char *value) {
  snek_object_t *obj = _new_snek_object();
  if (obj == NULL) {
    return NULL;
  }

  int len = strlen(value);
  char *dst = malloc(len + 1);
  if (dst == NULL) {
    free(obj);
    return NULL;
  }

  strcpy(dst, value);

  obj->kind = STRING;
  obj->data.v_string = dst;
  return obj;
}
