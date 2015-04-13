#include "actuators.h"
#include "common.h"
#include "switch.h"
#include "board.h"
#include "func.h"

static bool actuator_switch_init(struct actuator_t* a)
{
    Switch_Config();
    return true;
}

static bool actuator_switch_action(struct actuator_t* a)
{
    Switch_Action(a->value.value_bool);
    return true;
}

static struct actuator_t actuator_switch = {
    .actuator_name = "switch",
    .value_type = ACTUATOR_VALUE_BOOL,
    .driver_init = actuator_switch_init,
    .action = actuator_switch_action,
};

static struct actuator_t foo;

static struct actuator_t *actuators_foo[] = {
    &foo, //empty array is not allowed in MDK
#ifdef ENABLE_SWITCH
    &actuator_switch,
#endif
};

struct actuator_t **actuators = &actuators_foo[1]; //skip "foo"

const int actuators_count = sizeof(actuators_foo)/sizeof(struct actuator_t *)-1;

