import React, { useContext, useEffect, useState } from 'react';
import { useGameState } from './GlobalGameState';

interface payload {
  // type: "init" | "terminating" | "acknowledged"
  event: "move" 
  contents?: string
  tile?: string;
}

type Props = {
  children?: any;
}

type WSType = {
  socket: WebSocket | null;
  emit: (key: string, value: any) => void;
}

export const WSContext = React.createContext<WSType>({
  socket: null,
  emit: () => {}
});



export const WSProvider = ({ children }: Props) => {
  const [socket, setSocket] = useState<WebSocket| null>(null);
  const [socketConnected, setSocketConnected] = useState<boolean>(false);
  
  const { setHexTileState } = useGameState();
  
  useEffect(() => {
    const WS = new WebSocket(`ws://localhost:8080/ws/david`);
    WS.onopen = () => {
      setSocketConnected(true);
    }
    WS.onmessage = (message) => {
      const data:payload = JSON.parse(message.data) as payload
      console.log(data);
      switch(data.event) {
        case "move": {
          // set gamestate tile to be red
          const tile = data.tile;
          if(tile) {
            setHexTileState(tile, {
              user: null,
              color: "red"
            })
          }
          break;
        }
      }
    }
    setSocket(WS);
  },[])

  const emit = (key: string, data?: any) => {
    socket?.send(JSON.stringify({
      "event": key,
      "data": data
    }));
  }

  return (
    <WSContext.Provider
      value={{
        socket,
        emit
      }}
    >
      {children}
    </WSContext.Provider>
  )
}

export const useSocket = () => useContext(WSContext);