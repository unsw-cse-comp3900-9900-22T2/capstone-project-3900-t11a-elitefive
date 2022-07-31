import { text } from "stream/consumers";


type loginResp = {
  uid: string,
  token: string,
  username:string
} 
export async function login(email: string, password: string): Promise<loginResp| null> {
  
  const data = { email: email, password: password };  
  const response = await fetch('/login', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
      body: JSON.stringify(data),
    })
    
  // register succes or register failure 
  const response_text = await response.text()
  const response_json = JSON.parse(response_text);
  const { outcome, uid, session, username } = response_json.payload

  if(outcome == "failure") return null;

  return { uid, token: session, username }
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
  
  const response_json = await response.json()
    
  return response_json;
}

// Post to server with values from register field
export async function resetPassword(uid: string, oldpass: string, newpass: string){
   
  const data = { uid: uid, oldpass: oldpass, newpass: newpass};
   
  const response = await fetch('/api/resetpassword', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
  },
    body: JSON.stringify(data),
  })
  
  const response_json = await response.json()
    
  console.log(response_json)
    
  return response_json;
}

export async function getAllReplays() {
  const resp = await fetch(`/api/search/all`);
  if(!resp) return null;
  const data = await resp.json();
  if(!data) return null;
  return data
}


export async function querySnapShot(snapshotstring: string) {
  const resp = await fetch(`/api/search/snapshot?moves=${snapshotstring}`);
  if(!resp) return null;
  const data = await resp.json();
  if(!data) return null;

  return data
}

// Post to server with values from register field
export async function forgotPassword(email: string){
   
  const data = { email: email };
   
  const response = await fetch('/api/temppass', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
  },
    body: JSON.stringify(data),
  })
  
  // register succes or register failure 
  const response_json = await response.json()
  const { outcome, message } = response_json.payload

  return response_json;
}