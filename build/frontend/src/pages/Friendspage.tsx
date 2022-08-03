import { Box, Typography } from '@mui/material';
import { minHeight } from '@mui/system';
import { userInfo } from 'os';
import React, { useEffect, useState } from 'react'
import styled from 'styled-components';
import FriendListComponent from '../components/FriendListComponent';
import FriendRequest from '../components/FriendRequest';
import Button from '../components/ReusableButton';
import StyledInput from '../components/StyledInput';
import YavalathButton from '../components/YavalathButton';
import { useAlert } from '../global/GlobalAlert';
import { useAuth } from '../global/GlobalAuth';

type Props = {}

const Container = styled.div`
  display: flex;
  flex-direction: column;
`;

const Left = styled.div`
  width: 50%;
  height: 100vh;
  display: flex;
  flex-direction: column;

  margin: 50px;
  gap: 50px;
`;
const Right = styled.div`
  width: 40%;
  height: 100vh;
  display: flex;
  flex-direction: column;

  margin: 50px;
  gap: 50px;
`;

type BoxContainerProps = {
  minHeight?: number;
}
const BoxContainer = styled.div<BoxContainerProps>`
  background: var(--accent-dark);
  margin: 20px 0;
  border-radius: 5px;
  padding: 30px 50px;

  ${({minHeight}) => minHeight && `
    min-height: ${minHeight}px;
  `}
`

// const outgoing_mock = [
//   "bob",
//   "james"
// ]


// const incoming_mock = [
//   "bob",
//   "james"
// ]

// const friends_mock = [
//   ":(",
//   "loner",
// ]

type UserType = {
  id: string;
  username: string;
}

export default function FriendsPage({}: Props) {
  const { getUID } = useAuth();
  const [friends, setFriends] = useState<UserType[]>([]);
  const [outgoing, setOutgoing] = useState<UserType[]>([]);
  const [incoming, setIncoming] = useState<UserType[]>([]);

  const [interaction, setInteraction] = useState(0);

  const [quickAddInput, setQuickAddInput] = useState('');

  const { setError } = useAlert();
  
  useEffect(() => {
    fetch(`/api/friends?uid=${getUID()}`)
    .then(resp => resp.json())
    .then(data => {
      const {friends, outgoing, incoming} = data.payload;
      console.log(data.payload)
      if(friends) setFriends(friends);
      if(outgoing) setOutgoing(outgoing);
      if(incoming) setIncoming(incoming);
    })
  },[interaction]);


  const handleQuickAdd = () => {
    const data = { action: "add", from: getUID(), to: quickAddInput}
    fetch(`/friend`,{
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
        body: JSON.stringify(data),
    })
    .then(resp => resp.json())
    .then(data => {
      if(!data || !data.payload) {
        setError('response doesn\'t have payload');
        return;
      }
      if (data.payload.outcome == "failure") {
        setError(data.payload.message);
      }
      //forces a refresh
      setInteraction(prev => prev+1);
    })
  }

  const refresh = () => {
    setInteraction(prev => prev+1);
  }

  return (
    <Container>
      <YavalathButton/>
      <Box display="flex">
      <Left>
        <Box>
          <Typography variant="h4">Friends</Typography>
          <BoxContainer minHeight={250}>
            {
              friends.map((user) => (
                <FriendListComponent to={user.id} name={user.username} refresh={refresh}/>
              ))
            }
          </BoxContainer>
        </Box>
        <Box>
          <Typography variant="h4">Quick Add</Typography>
          <Box display="flex" justifyContent="space-between" margin="30px 0">
            <StyledInput size="small" value={quickAddInput} onChange={(e) => {setQuickAddInput(e.target.value)}}/>
            <Box display="flex" alignItems="center">
              <Button
                background='var(--accent-darker)'
                height={40}
                onClick={handleQuickAdd}
              >
                add
              </Button>
            </Box>
          </Box>
        </Box>
      </Left>
      <Right>
        <Box>
          <Typography variant="h4">Pending</Typography>
          <BoxContainer>
            {
              outgoing.map((user) => (
                <FriendRequest id={user.id} name={user.username} variant="incoming" refresh={refresh}/>
              ))
            }
          </BoxContainer>
        </Box>
        <Box>
          <Typography variant="h4">Requests</Typography>
          <BoxContainer>
            {
              incoming.map((user) => (
                <FriendRequest id={user.id} name={user.username} variant="outgoing" refresh={refresh}/>
              ))
            }
          </BoxContainer>
        </Box>
      </Right>
      </Box>
    </Container>
  )
}