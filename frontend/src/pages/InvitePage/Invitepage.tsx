import { Typography } from '@mui/material';
import React, { useEffect, useState } from 'react'
import styled from 'styled-components';
import Button from '../../components/ReusableButton';
import { useAuth } from '../../global/GlobalAuth';

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
`;

const FriendsBox = styled.div`
  width: 70%;
  height: 50%;
  background: var(--accent-dark);
`

const Friend = styled.div`
  display: flex;
  justify-content: space-between;
  margin: 10px;
`;

const Pending = styled.div`
  background: var(--background-color);
  display: flex;
  margin: 10px;
`;

export default function Invitepage({}: Props) {

  const [inviteRoomWS, setInviteRoomWS] = useState<WebSocket|undefined>();
  const [friends, setFriends] = useState<FriendType[]>([]);
  const [pending, setPending] = useState<FriendType[]>([]);

  const { getUID } = useAuth();

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
      event: string;
      friends_uid: number[] | null;
      friends_username: string[] | null;
      pending_uid: number[] | null;
      pending_username: string[] | null;
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
        console.log(payload);
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
  }

  return (
    <Container>
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
        {pending?.map((friend: FriendType) => {
          return (
            <Pending>
              <Typography variant="h5">{friend.username}</Typography>
              <Button onClick={handleAccept(friend.uid)}>accept</Button>
              <Button onClick={handleReject(friend.uid)}>reject</Button>
            </Pending>
          )
        })}
      </Right>
    </Container>
  )
}