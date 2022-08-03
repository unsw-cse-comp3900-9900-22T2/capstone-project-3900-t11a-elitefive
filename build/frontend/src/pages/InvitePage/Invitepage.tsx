import { Box, Typography } from '@mui/material';
import React, { useEffect, useState } from 'react'
import { useNavigate } from 'react-router-dom';
import styled from 'styled-components';
import Button from '../../components/ReusableButton';
import { useAuth } from '../../global/GlobalAuth';
import { isEqual } from 'lodash';
import { PlayerType } from '../../global/GlobalGameState';
import { YavalathButtonFixed } from '../../components/YavalathButton';

type Props = {}

type FriendType = {
  uid: number;
  username: string;
}

const Container = styled.div`
  display: flex;
  width: 100vw;
  height: 100vh;
`;


const Left = styled.div`
  width: 50vw;

  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 50px;

`;

const Right = styled.div`
  width: 50vw;
  background: var(--accent-dark);

  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 50px;
`;

const FriendsBox = styled.div`
  width: 70%;
  height: 50%;
  background: var(--accent-dark);
`

const PendingBox = styled.div`
  width: 70%;
  height: 50%;
  background: var(--background-color);
`

const Friend = styled.div`
  display: flex;
  justify-content: space-between;
  margin: 10px;
`;

const Pending = styled.div`
  background: var(--background-color);
  display: flex;
  justify-content: space-between;
  margin: 10px;
`;

export default function Invitepage({}: Props) {

  const [inviteRoomWS, setInviteRoomWS] = useState<WebSocket|undefined>();
  const [friends, setFriends] = useState<FriendType[]>([]);
  const [pending, setPending] = useState<FriendType[]>([]);

  const { getUID } = useAuth();
  const navigate = useNavigate();

  useEffect(() => {
    if(!inviteRoomWS) {
      JoinSocketPromise()
      .then(ws => {
        setInviteRoomWS(ws);
      })
    }
  }, [])

  const JoinSocketPromise = (): Promise<WebSocket> => {
    type Payload = {
      // friends
      event: string;
      friends_uid: number[] | null;
      friends_username: string[] | null;
      pending_uid: number[] | null;
      pending_username: string[] | null;

      // invite
      from_username: string;
      // decline
      from_uid: number;

      // match_created
      room_id: string;
      uids: number[];
    }
    return new Promise((resolve, reject) => {
      const ws: WebSocket = new WebSocket('ws://localhost:8080/ws/inviteroom')
      ws.onopen = () => {
        console.log("joined invite waiting room");
        ws?.send(JSON.stringify({
          "event":"subscribe",
          "data":{
            "uid": getUID(),
          },
        }))
      }
      ws.onmessage = (message) => {
        const payload: Payload = JSON.parse(message.data) as Payload;
        // console.log(payload);
        switch(payload.event) {
          case "friends": {
            // initialise initial state
            const { friends_uid, friends_username } = payload;
            if(friends_uid && friends_username) {
              let temp_friends = [] as FriendType[];
              friends_username.map((val, index) => {
                const friend = {
                  username: val,
                  uid: friends_uid[index]
                }
                temp_friends.push(friend);
              })
              setFriends(temp_friends);
            }
            const { pending_uid, pending_username } = payload;
            if(pending_uid && pending_username) {
              let temp_pending = [] as FriendType[];
              pending_username.map((val, index) => {
                const friend = {
                  username: val,
                  uid: pending_uid[index]
                }
                temp_pending.push(friend);
              })
              setPending(temp_pending);
            }
            break;
          }

          case "invite": {
            const { from_uid, from_username } = payload;
            if(from_uid) {
              const new_player = { uid: from_uid, username: from_username }
              // ignore duplicate
              // checks if there is duplicate
              // by deepEqual-ing the object
              setPending(prev => {
                let found = false;
                for(const player of prev) {
                  console.log(player)
                  console.log(new_player)
                  if(isEqual(player, new_player)) {
                    found = true;
                  }
                }
                if(!found) {
                  return [...prev, new_player]
                }
                return prev;
              });
            }
            break;
          }

          // case "declined": {
          //   const { from_uid } = payload;
          //   console.log(typeof from_uid)
          //   if(from_uid) {
          //     setPending(prev => prev.filter((player) => {
          //       console.log(prev);
          //       console.log(from_uid)
          //       return player.uid !== from_uid
          //     }));
          //   }
          //   break;
          // }

          case "match_created":{
            const { room_id , uids } = payload;
            const own_uid = getUID();
            if(!own_uid) break;
            if(uids.includes(parseInt(own_uid))) {
              ws?.close();
              navigate(`/game/${room_id}`);
            }
            break;
          }
        }
      }
      resolve(ws);
    })
  }

  const emit = (key: string, data?: any) => {
    inviteRoomWS?.send(JSON.stringify({
      "event": key,
      "data": data
    }));
  }

  const handleInvite = (uid: number) => () => {
    emit("send_invite", {
      "friend": String(uid),
      "uid": getUID()
    })
  }

  const handleAccept = (uid: number) => () => {
    emit("accept_invite", {
      "friend": String(uid),
      "uid": getUID()
    })
  }
  
  const handleReject = (uid: number) => () => {
    emit("decline_invite", {
      "friend": String(uid),
      "uid": getUID()
    })

    setPending(prev => prev.filter((player) => player.uid != uid));
  }

  return (
    <Container>
      <YavalathButtonFixed/>
      <Left>
        <Typography variant="h3">Invite Friends</Typography>
        <FriendsBox>
          {friends?.map((friend: FriendType) => {
            return(
              <Friend>
                <Button background="var(--accent-purple)" onClick={handleInvite(friend.uid)}>
                  invite
                </Button>
                <Typography variant="h5">{friend.username}</Typography>
              </Friend>
            )
          })}
        </FriendsBox>
      </Left>
      <Right>
        <Typography variant="h3">Your current Invites</Typography>
        <PendingBox>
          {pending?.map((friend: FriendType) => {
            return (
              <Pending>
                <Typography variant="h5">{friend.username}</Typography>
                <Box display="flex" flexDirection="row">
                  <Button background="#90E39A" onClick={handleAccept(friend.uid)} color="var(--accent-dark)">accept</Button>
                  <Button background="#CE4760" onClick={handleReject(friend.uid)}>reject</Button>
                </Box>
              </Pending>
            )
          })}
        </PendingBox>
      </Right>
    </Container>
  )
}