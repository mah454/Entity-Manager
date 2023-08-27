
#include <vector>
#include "repository/PersonRepository.h"
#include <syslog.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {

    openlog("entity-manager", LOG_PID, LOG_LOCAL0);

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

