#define __NEW_STARLET

#include <starlet.h>
#include <str$routines.h>
#include <iledef.h>
#include <uaidef.h>
#include <descrip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <mosquitto.h>
#include <mosquitto_plugin.h>


#define OKAY(STATUS) (((STATUS) & 1) != 0)


int mosquitto_auth_plugin_version(void)
{
    return (MOSQ_AUTH_PLUGIN_VERSION);
}

int mosquitto_auth_plugin_init(void **user_data,
			       struct mosquitto_auth_opt *auth_opts,
			       int auth_opt_count)
{
    /* No options */
    return (MOSQ_ERR_SUCCESS);
}

int mosquitto_auth_plugin_cleanup(void **user_data,
				  struct mosquitto_auth_opt *auth_opts,
				  int auth_opt_count)
{
    return (MOSQ_ERR_SUCCESS);
}

int mosquitto_auth_security_init(void **user_data,
				 struct mosquitto_auth_opt *auth_opts,
				 int auth_opt_count, bool reload)
{
    return (MOSQ_ERR_SUCCESS);
}

int mosquitto_auth_security_cleanup(void **user_data,
				    struct mosquitto_auth_opt *auth_opts,
				    int auth_opt_count, bool reload)
{
    return (MOSQ_ERR_SUCCESS);
}

int mosquitto_auth_acl_check(void **user_data, const char *username,
			     const char *topic, int access)
{
    /* For the moment we're assuming that if someone can log in then they can do all sorts of stuff */
    return (MOSQ_ERR_SUCCESS);
}

int mosquitto_auth_unpwd_check(void **user_data, const char *username,
			       const char *password)
{
    struct dsc$descriptor_s usr_dsc;
    struct dsc$descriptor_s pwd_dsc;
    int rv;
    unsigned char encrypt;
    unsigned long status;
    long long pwd_uaf;
    long long pwd_usr;
    unsigned short salt;

    ILE3 info[4] = {
	{sizeof(pwd_uaf), UAI$_PWD, &pwd_uaf, 0}
	,
	{sizeof(encrypt), UAI$_ENCRYPT, &encrypt, 0}
	,
	{sizeof(salt), UAI$_SALT, &salt, 0}
	,
	{0, 0, 0, 0}
    };

    if (username == NULL || password == NULL) {
       return (MOSQ_ERR_AUTH);
    }

    usr_dsc.dsc$w_length = strlen(username);
    usr_dsc.dsc$b_dtype = DSC$K_DTYPE_T;
    usr_dsc.dsc$b_class = DSC$K_CLASS_S;
    usr_dsc.dsc$a_pointer = strdup(username);

    assert(usr_dsc.dsc$a_pointer);

    pwd_dsc.dsc$w_length = strlen(password);
    pwd_dsc.dsc$b_dtype = DSC$K_DTYPE_T;
    pwd_dsc.dsc$b_class = DSC$K_CLASS_S;
    pwd_dsc.dsc$a_pointer = strdup(password);

    assert(pwd_dsc.dsc$a_pointer);

    str$upcase(&usr_dsc, &usr_dsc);
    str$upcase(&pwd_dsc, &pwd_dsc);

    rv = MOSQ_ERR_AUTH;  /* Assume badness */

    status = sys$getuai(0, 0, &usr_dsc, info, 0, 0, 0);

    if (OKAY(status)) {
	status =
	    sys$hash_password(&pwd_dsc, encrypt, salt, &usr_dsc,
			      (struct _generic_64 *) &pwd_usr);

	if (OKAY(status)) {
	    if (pwd_uaf == pwd_usr) {	/* Do the hashes match? */
		rv = MOSQ_ERR_SUCCESS;
	    }
	}
    }

    free(usr_dsc.dsc$a_pointer);
    free(pwd_dsc.dsc$a_pointer);

#ifdef MQAP_DEBUG
    if (rv == MOSQ_ERR_AUTH) {
	fprintf(stderr, "Invalid username or password (username \"%s\")\n",
		username);
    }
#endif
    return (rv);
}

int mosquitto_auth_psk_key_get(void **user_data, const char *hint,
			       const char *identity, char *key,
			       int max_key_len)
{
    return (MOSQ_ERR_AUTH);
}
