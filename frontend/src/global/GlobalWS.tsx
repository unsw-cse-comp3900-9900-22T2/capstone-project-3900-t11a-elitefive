import { getuid } from 'process';
import React, { useContext, useEffect, useState } from 'react';
import { useAuth } from './GlobalAuth';
import { useGameState, PlayerType } from './GlobalGameState';

interface payload {
  // type: "init" | "terminating" | "acknowledged"
  event: "move" | "moveconfirm" | "game_over" | "player_names" | "player3" | "pothole"
  contents?: string;
  tile?: string;
  winner?: string;
}

type Props = {
  children?: any;
  gameId: string | undefined;
}

type WSType = {
  socket: WebSocket | null;
  emit: (key: string, value: any) => void;
}

export const WSContext = React.createContext<WSType>({
  socket: null,
  emit: () => {}
});



export const WSProvider = ({ children, gameId }: Props) => {
  const [socket, setSocket] = useState<WebSocket| null>(null);
  const [socketConnected, setSocketConnected] = useState<boolean>(false);
  
  const { 
    playMove, 
    setHexTileState, 
    playerJoin, 
    getPlayerInfo, 
    setWinner, 
    setPlayerName, 
    setPlayerElo
  } = useGameState();
  const { getUID } = useAuth();
  
  useEffect(() => {
    const WS = new WebSocket(`ws://localhost:8080/ws/game/${gameId}`);
    WS.onopen = () => {
      setSocketConnected(true);
      // playerJoin({uid: "abc", color: "#ea5f95"});
      // playerJoin({uid: "BOT", color: "#4ee478"});
      // playerJoin({uid: "p3", color: "#14d0e1"});
      playerJoin({uid: "abc", color: "#f33880"});
      playerJoin({uid: "BOT", color: "#1de254"});
      playerJoin({uid: "p3", color: "#00b9ca"});
      playerJoin({uid: "pothole", color: "#12121"})
    }
    WS.onmessage = (message) => {
      const payload:payload = JSON.parse(message.data) as payload
      console.log(payload);
      switch(payload.event) {
        case "move": {
          // set gamestate tile to be red
          const move = payload.tile;
          if(move) {
            // this is hardcoded for SINGLE PLAYER will need
            // backend player join broadcast to correctly work
            // playerJoin({uid: "BOT", color: "blue"});
            const playerColor = getPlayerInfo("BOT")?.color;
            if(!playerColor) break;
            setHexTileState(move, {
              user: "BOT",
              color: playerColor
            })
          }
          break;
        }
        case "moveconfirm": {
          const payload:payload = JSON.parse(message.data) as payload
          const hexKey = payload.tile;
          if (hexKey) {
            playMove("abc", hexKey);
          }
          break;
        }
        case "player3" : {
          const payload:payload = JSON.parse(message.data) as payload
          const hexKey = payload.tile;
          if (hexKey) {
            playMove("p3", hexKey);
          }
          break;
        }
        case "pothole" : {
          const payload:payload = JSON.parse(message.data) as payload
          const hexKey = payload.tile;
          if (hexKey) {
            playMove("pothole", hexKey);
          }
          break;
        }
        case "game_over": {
          const payload:payload = JSON.parse(message.data) as payload
          const winner = payload.winner;
          if(winner) {
            setWinner(winner);
          }
          WS.close();
          break;
        }
        case "player_names": {
          const payload = JSON.parse(message.data);
          console.log(payload)
          const { player_name, elos } = payload;
          setPlayerName(0, player_name[0]);
          setPlayerElo(0, elos[0])
          setPlayerName(1, player_name[1]);
          setPlayerElo(1, elos[1])
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
        emit,
      }}
    >
      {children}
    </WSContext.Provider>
  )
}

export const useSocket = () => useContext(WSContext);