#ifndef DEBUG_H
#define DEBUG_H 

#ifdef DEBUG
    #define DEBUG_MSG(MSG, ...)                                                                                                             \
    do {                                                                                                                                    \
        fprintf(stderr, "\n\t[DEBUG] File : %s - Line : %d - Function : %s() : " MSG "\n", __FILE__, __LINE__, __func__, ## __VA_ARGS__);   \
    } while(0);
#else
    #define DEBUG_MSG(MSG, ...)
#endif

#ifdef DEBUG
    #define DEBUG_PRINTF(MSG, ...)               \
    do {                                         \
        fprintf(stderr, MSG, ## __VA_ARGS__);    \
    } while(0);
#else
    #define DEBUG_PRINTF(MSG, ...)
#endif

#endif
