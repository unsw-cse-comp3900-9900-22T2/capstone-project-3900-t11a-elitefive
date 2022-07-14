import React, 
{ 
  useEffect, 
  useState,
  useContext,
} from 'react';


// types
export interface IAuth {
  login: () => Promise<unknown>;
  logout: () => void;
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
  login: () => new Promise((reject, resolve) => {}),
  logout: () => {}, 
});

export const AuthProvider = ({ children }: Props) => {
  const [auth, setAuth] = useState(defaultAuthState);
  
  const login = async () => {
    // TODO
  }

  const logout = () => {
    // TODO
  }

  // on start
  useEffect(() => {
    const token: string|null = getStoredToken();
    if(token) {
      setAuth(prevState => ({
        ...prevState,
        token: token,
      }));
    }
  }, [])

  return (
    <AuthContext.Provider
      value={{
        login,
        logout,
      }}
    >
      {children}
    </AuthContext.Provider>
  )
}

// create hook
export const useAuth = () => useContext(AuthContext);