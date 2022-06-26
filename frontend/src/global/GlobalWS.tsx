import React, { useContext, useEffect, useState } from 'react';
import WSClient from '../api/ws/client';
import GameState from '../classes/Gamestate';

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
    // create blank gamestate
    const gamestate = new GameState();
    setSocket(new WSClient(gamestate));
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