import React, 
{ 
  useEffect, 
  useState,
  useContext,
} from 'react';

import * as API from '../api/rest';


// types
export interface IAuth {
  login: (email: string, password: string) => Promise<unknown>;
  logout: () => void;
  getUID: () => string | null;
  getToken: () => string |  null;
  isLogged: () => boolean;
}

type Props = {
  children?: any;
}


// extract from localstorage

export const getStoredUID = (): string|null => window.localStorage.getItem("uid")
export const setStoredUID = (uid: string): void => { window.localStorage.setItem("uid", uid)}
export const removeStoredUID = () =>  { window.localStorage.removeItem("uid"); }

export const getStoredUsername = (): string|null => window.localStorage.getItem("username")
export const setStoredUsername = (username: string): void => { window.localStorage.setItem("username", username)}
export const removeStoredUsername = () =>  { window.localStorage.removeItem("username"); }


export const getStoredToken = (): string|null => window.localStorage.getItem("session-token")
export const setStoredToken = (token: string): void => { window.localStorage.setItem("session-token", token)}
export const removeStoredToken = () =>  { window.localStorage.removeItem("session-token"); }


// default authenication state
const defaultAuthState = {
  uid: '',
  sessionToken: '',
}

export const AuthContext = React.createContext<IAuth>({
  ...defaultAuthState,
  login: (email: string, password: string) => new Promise((resolve, reject) => {}),
  logout: () => {}, 
  getUID: () => null,
  getToken: () => null,
  isLogged: () => false,
});

export const AuthProvider = ({ children }: Props) => {
  const [auth, setAuth] = useState(defaultAuthState);
  
  const login = (email:string, password:string) => {
    return new Promise( async (resolve, reject) => {

      const resp = await API.login(email, password);
      if(!resp) {
        reject(false);
        return;
      }
      const {uid, token, username} = resp;
      setAuth({uid, sessionToken: token})
      setStoredToken(token)
      setStoredUID(uid)
      setStoredUsername(username)
      resolve(true);
    })
  }

  const logout = () => {
    setAuth(defaultAuthState);
    removeStoredToken()
    removeStoredUID()
    removeStoredUsername()

    console.log(auth);
  }

  const getUID = () => {
    return getStoredUID();
  }

  const getToken = () => {
    return getStoredToken();
  }

  const isLogged = () => {
    return (!!auth.uid && !!auth.sessionToken) || (!!getStoredToken() && !!getStoredUID());
  }

  // on start
  useEffect(() => {
    const token: string|null = getStoredToken();
    const uid: string | null = getStoredUID();
    if(token && uid) {
      setAuth(prevState => ({
        ...prevState,
        sessionToken: token,
        uid: uid
      }));
    }
  }, [])

  return (
    <AuthContext.Provider
      value={{
        login,
        logout,
        getUID,
        getToken,
        isLogged
      }}
    >
      {children}
    </AuthContext.Provider>
  )
}

// create hook
export const useAuth = () => useContext(AuthContext);