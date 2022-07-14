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
      const {uid, token} = resp;
      setAuth({uid, sessionToken: token})
      setStoredToken(token)
      setStoredUID(uid)
      resolve(true);
    })
  }

  const logout = () => {
    setAuth(defaultAuthState);
    removeStoredToken()
    removeStoredUID()

    console.log(auth);
  }

  const getUID = () => {
    return auth.uid;
  }

  const getToken = () => {
    return auth.sessionToken;
  }

  const isLogged = () => {
    return !!auth.uid && !!auth.sessionToken;
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