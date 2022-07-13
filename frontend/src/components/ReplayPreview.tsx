import React from 'react'
import styled from 'styled-components';
import { Box } from '@mui/material';

type Props = {
  mode: string;
  result: string;
  date: string;
  img: string;
}

const Container = styled.div`
  width: 200px;
  height: 180px;
  background: var(--accent-darker);
  border-radius: 5px;
  
  display: flex;
  flex-direction: column;
  margin: 50px;
`;

type TagProp = {
  padding?: string;
  color?: string;
}
const Tag = styled.div<TagProp>`
  border-radius: 20px;
  max-height: 25px;
  padding: ${props=> props.padding ? props.padding: "1px 8px"};
  background: ${props => props.color};

  font-size: 13px;
  text-align: center;
`;

const ImagePlaceholder = styled.div`
  width: 100%;
  height: 200px;
  background: var(--accent-dark);
`

const BottomTags = styled.div`
  display: flex;
  padding: 10px 30px;
`;

export default function ReplayPreview({mode, result, date, img}: Props) {
  return (
    <Container>
      <Box display="flex" justifyContent="flexStart" margin="10px">
        <Tag color="grey">{mode}</Tag>
      </Box>
      <ImagePlaceholder/>
      <BottomTags>
        <Box marginRight="auto">
          <Tag color={ result=="win" ? "green": "red" } padding="1px 15px">{result}</Tag>
        </Box>
        <Tag color="grey" padding="1px 15px">{date}</Tag>
      </BottomTags>
    </Container>
  )
}