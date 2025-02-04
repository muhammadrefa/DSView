/*
 * This file is part of the DSView project.
 * DSView is based on PulseView.
 * 
 * Copyright (C) 2022 DreamSourceLab <support@dreamsourcelab.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef DEVICE_AGENT_H
#define DEVICE_AGENT_H
 
#include <glib.h>
#include <stdint.h>
#include <libsigrok.h>
#include <QString>
#include <vector>

class IDeviceAgentCallback
{
    public:
        virtual void DeviceConfigChanged()=0;
};

class DeviceAgent
{
public:
    DeviceAgent();

    void update();

    inline bool have_instance(){
        return _dev_handle != NULL_HANDLE;
    }

    inline QString name(){
        return _dev_name;
    }

    inline QString path(){
        return _path;
    }

    inline QString driver_name(){
        return _driver_name;
    }

    inline ds_device_handle handle(){
        return _dev_handle;
    }

    struct sr_dev_inst* inst();

    inline bool is_file(){
        return _dev_type == DEV_TYPE_FILELOG;
    }

    inline bool is_demo(){
        return _dev_type == DEV_TYPE_DEMO;
    }

    inline bool is_hardware(){
        return _dev_type == DEV_TYPE_USB;
    }

    inline void set_callback(IDeviceAgentCallback *callback){
        _callback = callback;
    }

    GVariant* get_config(const sr_channel *ch, const sr_channel_group *group, int key);

    bool set_config(sr_channel *ch, sr_channel_group *group, int key, GVariant *data);

	GVariant* get_config_list(const sr_channel_group *group, int key);

	bool enable_probe(const sr_channel *probe, bool enable);

    bool enable_probe(int probe_index, bool enable);

    bool set_channel_name(int ch_index, const char *name);

    /**
	 * @brief Gets the sample limit from the driver.
	 *
	 * @return The returned sample limit from the driver, or 0 if the
	 * 	sample limit could not be read.
	 */
	uint64_t get_sample_limit();

     /**
     * @brief Gets the sample rate from the driver.
     *
     * @return The returned sample rate from the driver, or 0 if the
     * 	sample rate could not be read.
     */
    uint64_t get_sample_rate();

       /**
     * @brief Gets the time base from the driver.
     *
     * @return The returned time base from the driver, or 0 if the
     * 	time base could not be read.
     */
    uint64_t get_time_base();

     /**
     * @brief Gets the sample time from the driver.
     *
     * @return The returned sample time from the driver, or 0 if the
     * 	sample time could not be read.
     */
    double get_sample_time();

    /**
     * @brief Gets the device mode list from the driver.
     *
     * @return The returned device mode list from the driver, or NULL if the
     * 	mode list could not be read.
     */
    const GSList *get_device_mode_list();

    /**
     * Check whether the trigger exists
     */
    bool is_trigger_enabled();

    bool have_enabled_channel();

    bool get_status(struct sr_status &status, gboolean prg); 

    GSList* get_channels();

    /**
     * Start collect data.
     */
    bool start();

    /**
     * Stop collect
     */
    bool stop();

    /**
     * Stop and close.
    */
    void release();

    bool is_collecting();

    inline bool is_new_device(){
        return _is_new_device;
    }

private:
    void config_changed();
    bool is_in_history(ds_device_handle dev_handle);

    //---------------device config-----------/
public:
  int get_work_mode(); 

  const struct sr_config_info* get_config_info(int key);

  bool get_device_status(struct sr_status &status, gboolean prg);

  struct sr_config* new_config(int key, GVariant *data);

  void free_config(struct sr_config *src);


private:
    ds_device_handle _dev_handle;
    int         _dev_type;
    QString     _dev_name;
    QString     _driver_name;
    QString     _path;
    bool        _is_new_device;
    struct sr_dev_inst  *_di;
    std::vector<ds_device_handle> _history_handles;
    IDeviceAgentCallback *_callback;
};


#endif