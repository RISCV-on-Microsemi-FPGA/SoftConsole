
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

const uint8_t * sys_cfg_get_mac_address(void);
void get_user_eth_config_choice(uint32_t* speed, uint32_t* filter);
void set_user_eth_speed_choice(uint32_t speed_choice);
void set_user_eth_filter_choice(uint32_t filter_choice);

#endif  /* SYSTEM_CONFIG_H_ */
