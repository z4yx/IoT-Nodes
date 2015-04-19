#include "actuators.h"
#include "common.h"
#include "switch.h"
#include "irda.h"
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

static bool actuator_IR_init(struct actuator_t* a)
{
    IRDA_INIT(); 
    return true;
}

static bool actuator_IR_action(struct actuator_t* a)
{
    static czx_vu8 irda_data[112];
    int val = a->value.value_int;
    if(val < 0){ //negative number indicates learning
        val = - val;
        DBG_MSG("Learn signal %d", val);

        IRDA_learn_start();
        while(!IRDA_BUSY_S());
        DBG_MSG("busy high");
        u8 len = IRDA_learn_data_in_out(irda_data);
        DBG_MSG("End learn len=%d", len);
    }else if(val > 0){
        DBG_MSG("Emit signal %d", val);

        DBG_MSG("Start tx");
        IRDA_tx_data(irda_data,sizeof(irda_data));
        DBG_MSG("End tx");
    }
    return true;
}

static struct actuator_t actuator_IR = {
    .actuator_name = "IR",
    .value_type = ACTUATOR_VALUE_INT,
    .flags = ACTUATOR_FLAG_TRIGGER,
    .driver_init = actuator_IR_init,
    .action = actuator_IR_action,
};

static struct actuator_t foo;

static struct actuator_t *actuators_foo[] = {
    &foo, //empty array is not allowed in MDK
#ifdef ENABLE_SWITCH
    &actuator_switch,
#endif
#ifdef ENABLE_IR
    &actuator_IR,
#endif
};

struct actuator_t **actuators = &actuators_foo[1]; //skip "foo"

const int actuators_count = sizeof(actuators_foo)/sizeof(struct actuator_t *)-1;

