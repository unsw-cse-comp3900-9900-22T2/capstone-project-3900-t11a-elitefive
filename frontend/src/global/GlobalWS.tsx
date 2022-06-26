import React, { useContext, useEffect, useState } from 'react';
import WSClient from '../api/ws/client';

type Props = {
  children?: any;
}

type WSType = {
  socket: WSClient | null;
}

export const WSContext = React.createContext<WSType>({
  socket: null,
});

export const WSProvider = ({ children }: Props) => {
  const [socket, setSocket] = useState<WSClient | null>(null);

  useEffect(() => {
    setSocket(new WSClient());
  },[])

  return (
    <WSContext.Provider
      value={{
        socket,
      }}
    >
      {children}
    </WSContext.Provider>
  )
}

export const useSocket = () => useContext(WSContext);