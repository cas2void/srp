/*******************************************************************************
 * File   : assert_ig.h
 * Content: Define the assert macro for debug output.
 *
 * Coder  : Ean Jee
 * Time   : 2011-1-9 22:09
 ******************************************************************************/

#ifdef NDEBUG
    /* If not debugging, assert does nothing. */
    #define ASSERT(e)
    #define ASSERTMSG(e, msg)

#else
    /* Debuging enabled. */
    #define GARBAGE 0xA3

    extern void IgAssert(const char *fileName, int lineNo);
    extern void IgAssertMsg(char *msg);

    #define ASSERT(e)            \
    if (e)                       \
    {}                           \
    else                         \
        IgAssert(__FILE__, __LINE__)

    #define ASSERTMSG(e, msg)    \
    if (e)                       \
    {}                           \
    else                         \
        IgAssertMsg(msg)

#endif /* NDEBUG */
