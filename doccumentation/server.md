## /login

Login Success
```
{
    "event": "login",
    "action": "login",
    "payload" : {
        "outcome" : "success"
        "uid" : "<uid>"
        "email" : "<email>"
        "session" : "<session_token>"

}
```

Logout
```
{
    "event": "login",
    "action": "logout",
    "payload" : {
        "uid" : "<uid>"
}
```

Login Failure - Incorrect Password
```
{
    "event": "login",
    "action": "login",
    "payload" : {
        "outcome" : "failure"
        "message" : "incorrect password"
}
```

Login Failure - Email not in database
```
{
    "event": "login",
    "action": "login",
    "payload" : {
        "outcome" : "failure"
        "message" : "email not in database"
}
```


## /register

Register Success

```
{
    "event": "register",
    "action": "register",
    "payload" : {
        "outcome" : "success"
}
```

Register Failure

```
{
    "event": "register",
    "action": "register",
    "payload" : {
        "outcome" : "failure"
}
```

## Friends

Get friends

```
{
	"event": "friends",
	"action": "get",
	"payload": {
		"user": "<uid>",
		"friends": [{
			"uid": "<uid>",
			"username": "<username>"
		}, ...]
	}
}
```

Add friend success

```
{
    "event": "friends",
    "action": "add",
    "payload" : {
        "outcome" : "success"
    }
}
```

Add friend failure

```
{
    "event": "friends",
    "action": "add",
    "payload" : {
        "outcome" : "failure"
    }
}
```

Delete friend success

```
{
    "event": "friends",
    "action": "delete",
    "payload" : {
        "outcome" : "success"
    }
}
```

Delete friend failure

```
{
    "event": "friends",
    "action": "delete",
    "payload" : {
        "outcome" : "failure"
    }
}
```










