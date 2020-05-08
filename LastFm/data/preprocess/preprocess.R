library(data.table)
data<-as.data.frame(fread("/data/usersha1-artmbid-artname-plays.tsv"))
#process music id
musicid <- data.frame(unique(data$V2))
df_musicid = data.frame("newid" = seq(1,nrow(musicid)),"musicid" = musicid)
head(df_musicid)
merge_musicid <- merge(data, df_musicid, by.x = "V2", by.y = "unique.data.V2.")
merge_musicid1 <- merge_musicid[!(!is.na(merge_musicid$V2) & merge_musicid$V2==""), ]
head(merge_musicid)
#process user id
userid <- data.frame(unique(data$V1))
df_userid = data.frame("newuserid" = seq(1,nrow(userid)),"userid" = userid)
colnames(df_userid)
merge_userid <- merge(merge_musicid1, df_userid, by.x = "V1", by.y = "unique.data.V1.")
#process ratings
library(dplyr)
merge_rating <- merge_userid %>%
  mutate(rating = case_when(V4 < 30 ~ 1,
                            V4 >= 30 & V4 < 70 ~ 2,
                            V4 >= 70 & V4 < 140 ~ 3,
                            V4 >= 140 & V4 < 300 ~ 4,
                            V4 >= 300 ~ 5))
colnames(merge_rating)
merge <- merge_rating[,c("newuserid","newid","rating")]
merge <- merge[order(merge$newuserid),]
write.table(merge,'/data/merge.csv', sep=",", row.names = FALSE, col.names = FALSE)
