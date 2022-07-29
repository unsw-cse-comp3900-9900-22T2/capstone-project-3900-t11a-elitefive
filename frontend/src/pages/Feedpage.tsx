import { Typography } from '@mui/material';
import React, { useEffect, useState } from 'react'
import { useNavigate } from 'react-router-dom';
import styled from 'styled-components';
import { useAuth } from '../global/GlobalAuth'

type Props = {}

const FeedContainer = styled.div<FeedDataType>`
  width: 60vw;
  height: 150px;
  margin: 50px;
  background: var(--accent-dark);

  display: flex;
  justify-content: center;
  align-items: center;
  
  cursor: ${props => props["has-link"] ? "pointer": ""};
`;

const Container = styled.div`
  min-height: 100vh;
`;

type FeedDataType = {
  "has-link"?: boolean;
  link?: string;
  message: string;
}

export default function Feedpage({}: Props) {
  const [feedData, setFeedData] = useState<FeedDataType[]>([])
  const { getUID } = useAuth();
  const navigate = useNavigate();
  useEffect(() => {
    fetch(`/api/social/feed?uid=${getUID()}`)
    .then(resp => resp.json())
    .then(data => {
      setFeedData(data);
    })
  },[])

  const handleFeedClick = (feed: FeedDataType) => () => {
    if(!feed["has-link"]) return;
    if(!feed || !feed.link) return;
    
    navigate(feed.link);
  }
  return (
    <Container>
      {feedData.map((feed: FeedDataType) => {
        // console.log(feed)
        return (
          <FeedContainer 
          onClick={handleFeedClick(feed)}
          {...feed}
          >
            <Typography variant="h5">
              {feed.message}
            </Typography>
          </FeedContainer>
        )
      })}
    </Container>
  )
}