## /login

Login Success
```
{
    "event": "login",
    "action": "login",
    "payload" : {
        "outcome" : "success"
        "uid" : "<uid>"
        "session" : "<session_token>"
        "email" : "<email>"
        "friends": [<array of friends UIDs>]
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

Login Failure
```
{
    "event": "login",
    "action": "login",
    "payload" : {
        "outcome" : "failure"
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




