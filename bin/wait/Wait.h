// Wait.h
#ifndef __BIN_WAIT_WAIT_H
#define __BIN_WAIT_WAIT_H

#include <POSIXApplication.h>

/**
 * @addtogroup bin
 * @{
 */

/**
 * Wait for a background process to finish.
 */
class Wait : public POSIXApplication
{
  public:
    /**
     * Constructor
     *
     * @param argc Argument count
     * @param argv Argument values
     */
    Wait(int argc, char **argv);

    /**
     * Destructor
     */
    virtual ~Wait();

    /**
     * Execute the application.
     *
     * @return Result code
     */
    virtual Result exec();
};

/**
 * @}
 */

#endif /* __BIN_WAIT_WAIT_H */

