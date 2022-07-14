import { text } from "stream/consumers";

export async function login(email: string, password: string) {
  // fetch("/login");
  
  const data = { email: email, password: password };

  
  const response = await fetch('/login', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
      body: JSON.stringify(data),
    })
    
    // register succes or register failure 
    const response_json = await response.json()
    console.log(response_json)
  
}

// Post to server with values from register field
export async function register(username: string, password: string, email: string){
  console.log(username)
   
  const data = { username: username, password: password, email: email };
   
  const response = await fetch('/register', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
  },
    body: JSON.stringify(data),
  })
  
  // register succes or register failure 
  const response_json = await response.json()
  console.log(response_json)
  
}