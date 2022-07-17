import { text } from "stream/consumers";

type loginResp = {
  uid: string,
  token: string
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
  const { outcome, uid, session } = response_json.payload

  if(outcome == "failure") return null;

  return { uid, token: session }
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
  const { outcome } = response_json.payload
  if(outcome == "success") return true;
  return false;
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