import { Alert, Box } from '@mui/material';
import React, 
{ 
  useEffect, 
  useState,
  useContext,
} from 'react';

import * as API from '../api/rest';


// types
export interface IError {
  setError: (msg: string) => void;
}

type Props = {
  children?: any;
}

export const AlertContext = React.createContext<IError>({
  setError: (msg: string) => {},
});

export const AlertProvider = ({ children }: Props) => {

  const [isAlert, setIsAlert] = useState<boolean>(false);
  const [severity, setSeverity] = useState<"error"|"warning"|"info"|"success">("info");
  const [alertMsg, setAlertMsg] = useState<string>('');

  const setError = (msg: string) => {
    setIsAlert(true);
    setSeverity("error");
    setAlertMsg(msg);

    setTimeout(() => {
      clearAlert();
    }, 2000)
  }

  const clearAlert = () => {
    setIsAlert(false);
    setAlertMsg('');
  }

  return (
    <AlertContext.Provider
      value={{
        setError,
      }}
    >
      {isAlert && (
        <Box position="absolute" left="50%">
          <Alert severity={severity}>{alertMsg}</Alert>
        </Box>
      )}
      {children}
    </AlertContext.Provider>
  )
}

// create hook
export const useAlert = () => useContext(AlertContext);