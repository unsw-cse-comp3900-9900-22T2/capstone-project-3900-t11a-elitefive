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




